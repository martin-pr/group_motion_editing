#include "empty.h"

using std::cout;
using std::endl;

namespace scenarios {

empty::empty(const boost::property_tree::ptree& config) : scenario<empty>(config) {
}

std::string empty::type() {
	return "empty";
}

}
