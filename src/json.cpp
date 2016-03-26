#include "json.h"

#include <iostream>
#include <unordered_map>

#include <boost/property_tree/json_parser.hpp>

#include "tools.h"

using std::cout;
using std::endl;

namespace {
	// visitor for all references ($ref child keys)
	void visitRefs(boost::property_tree::ptree& tree, std::function<void(boost::property_tree::ptree& current, const boost::property_tree::ptree& parent)> visitor) {
		for(auto& c : tree) {
			// references have $ref as key, and no children
			auto ref = c.second.get_child_optional("$ref");
			if(ref && ref->empty())
				visitor(c.second, tree);

			// and continue recursively
			visitRefs(c.second, visitor);
		}
	}
}

boost::property_tree::ptree read_json(const boost::filesystem::path& p) {
	boost::property_tree::ptree tree;

	// read the original content
	boost::property_tree::read_json(p.string(), tree);

	// for references support, we might need to read other files
	//   -> lets cache them, to make sure we don't parse each file more than once
	std::unordered_map<std::string, boost::property_tree::ptree> cache;

	// find all $refs, in accordance to http://json-spec.readthedocs.org/en/latest/reference.html
	visitRefs(tree, [&tree, &p, &cache](boost::property_tree::ptree& ref, const boost::property_tree::ptree& parent) {
		const std::string path = ref.get_child("$ref").data();

		// empty references not allowed
		if(path.length() < 2)
			throw std::runtime_error("found an empty JSON reference");
		// if data start with #, they're local references
		else if(path[0] == '#') {

			// root path
			if(path[1] == '/')
				ref = tree.get_child(boost::property_tree::path(path.substr(2), '/'));
			// relative path
			else
				ref = parent.get_child(boost::property_tree::path(path.substr(1), '/'));
		}
		else {
			auto pos = path.find('#');
			if(pos == std::string::npos)
				throw std::runtime_error("no # symbol in a reference");
			if(pos >= path.length()-1)
				throw std::runtime_error("no path after # symbol in a reference");

			// make a canonical path
			boost::filesystem::path can = boost::filesystem::canonical(path.substr(0, pos), p.parent_path());
			auto it = cache.find(can.string());
			if(it == cache.end()) {
				it = cache.insert(std::make_pair(can.string(), boost::property_tree::ptree())).first;
				boost::property_tree::read_json(can.string(), it->second);
			}
			assert(it != cache.end());

			// root path
			if(path[pos+1] == '/')
				ref = it->second.get_child(boost::property_tree::path(path.substr(pos+2), '/'));
			// relative path
			else
				throw std::runtime_error("relative references for external files not allowed");
		}
	});

	return tree;
}
