#pragma once

#include <iostream>

#include <boost/property_tree/ptree.hpp>

namespace boost { namespace property_tree {

std::ostream& operator << (std::ostream& out, const ptree& val);

} }
