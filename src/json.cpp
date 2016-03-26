#include "json.h"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>

#include "tools.h"

using std::cout;
using std::endl;

namespace {
	// visitor for all references
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

	// find all $refs
	visitRefs(tree, [&tree](boost::property_tree::ptree& ref, const boost::property_tree::ptree& parent) {
		const std::string path = ref.get_child("$ref").data();

		// empty references not allowed
		if(path.length() < 2)
			throw std::runtime_error("found empty JSON reference");
		// if data start with #, they're local references
		else if(path[0] == '#') {

			// root path
			if(path[1] == '/')
				ref = tree.get_child(boost::property_tree::path(path.substr(2), '/'));
			// relative path
			else
				ref = parent.get_child(boost::property_tree::path(path.substr(1), '/'));
		}
		else
			throw std::runtime_error("multi-file references not allowed (yet)");
	});

	return tree;
}
