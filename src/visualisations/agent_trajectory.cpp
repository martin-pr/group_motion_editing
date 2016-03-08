#include "agent_trajectory.h"

namespace visualisations {

agent_trajectory::agent_trajectory(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_render = config.get("render", std::string("line"));
	m_radius = config.get("r", 5.0f);
}

agent_trajectory::~agent_trajectory() {

}

void agent_trajectory::draw(std::ostream& svg, const agents& input, const scenarios::base& scenario) const {
	if(m_render == "line")
		for(unsigned a=0; a<input.agent_count(); ++a) {
			svg << "<polyline points=\"";
			for(unsigned f=0; f<input.frame_count(); ++f)
				svg << input[a][f].position.x << "," << input[a][f].position.y << " ";
			svg << "\" style=\"" << m_style << "\" />" << std::endl;
		}

	if(m_render == "circle") {
		for(unsigned a=0; a<input.agent_count(); ++a)
			for(unsigned f=0; f<input.frame_count(); ++f)
				svg << "<circle cx=\"" << input[a][f].position.x << "\" "
						"cy=\"" << input[a][f].position.y << "\" "
						"r=\"" << m_radius << "\" "
						"style=\"" << m_style << "\" />" << std::endl;
	}
}

const std::string agent_trajectory::type() {
	return "agent_trajectory";
}

}