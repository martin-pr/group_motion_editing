#include "agent_trajectory.h"

namespace visualisations {

agent_trajectory::agent_trajectory(const boost::property_tree::ptree& config) {

}

agent_trajectory::~agent_trajectory() {

}


const std::string agent_trajectory::type() {
	return "agent_trajectory";
}

}
