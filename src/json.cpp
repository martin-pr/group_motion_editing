#include "json.h"

#include <boost/property_tree/json_parser.hpp>

boost::property_tree::ptree read_json(const boost::filesystem::path& p) {
	boost::property_tree::ptree tree;

	boost::property_tree::read_json(p.string(), tree);

	return tree;
}
