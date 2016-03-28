#include "output_trajectory.h"

namespace visualisations {

output_trajectory::output_trajectory(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_render = config.get("render", std::string("line"));
	m_radius = config.get("r", 5.0f);
}

output_trajectory::~output_trajectory() {

}

void output_trajectory::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const {
	if(m_render == "line")
		for(unsigned a=0; a<output.agent_count(); ++a) {
			svg << "<polyline points=\"";
			for(unsigned f=0; f<output.frame_count(); ++f)
				svg << output[a][f].position.x << "," << output[a][f].position.y << " ";
			svg << "\" style=\"" << m_style << "\" />" << std::endl;
		}

	if(m_render == "circle") {
		for(unsigned a=0; a<output.agent_count(); ++a)
			for(unsigned f=0; f<output.frame_count(); ++f)
				svg << "<circle cx=\"" << output[a][f].position.x << "\" "
						"cy=\"" << output[a][f].position.y << "\" "
						"r=\"" << m_radius << "\" "
						"style=\"" << m_style << "\" />" << std::endl;
	}

	if(m_render == "spline")
		for(unsigned a=0; a<output.agent_count(); ++a) {
			svg << "<polyline points=\"";
			for(unsigned f=0; f<=(output.frame_count()-1)*6; ++f) {
				auto fr = output[a].interpolated((float)f / 6.0f);

				svg << fr.position.x << "," << fr.position.y << " ";
			}
			svg << "\" style=\"" << m_style << "\" />" << std::endl;
		}
}

const std::string output_trajectory::type() {
	return "output_trajectory";
}

}
