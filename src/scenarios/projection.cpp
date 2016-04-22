#include "projection.h"

namespace scenarios {

std::string projection::type() {
	return "projection";
}

projection::projection(const boost::property_tree::ptree& config) : scenario<projection>(config), curve(config){
}

agents projection::apply(const agents& source) const {
	return source;
}

}
