#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>

boost::property_tree::ptree read_json(const boost::filesystem::path& p);
