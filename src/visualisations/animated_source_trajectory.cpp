#include "animated_source_trajectory.h"

using std::endl;

namespace visualisations {

animated_source_trajectory::animated_source_trajectory(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_radius = config.get("r", 5.0f);
	m_duration = config.get("duration", 5.0f);
}

animated_source_trajectory::~animated_source_trajectory() {

}

void animated_source_trajectory::draw(std::ostream& svg, const agents& input, const scenarios::base& scenario) const {
	svg << "<style type=\"text/css\">" << endl;
	for(unsigned a=0; a<input.agent_count(); ++a) {
		svg << "  @keyframes agent_" << a << "_anim {" << endl;
		for(unsigned f=0; f<input.frame_count(); ++f) {
			const float t = (float)f / (float)(input.frame_count()-1) * 100;
			// svg << "    " << t << "% { left:" << input[a][f].position.x << "px; top: " << input[a][f].position.y << "px; }" << endl;
			svg << "    " << t << "% { transform: translateX(" << input[a][f].position.x << "px) translateY(" << input[a][f].position.y << "px); }" << endl;
		}
		svg << "  }" << endl;

		svg << "  #agent_" << a << " {" << endl;
		svg << "    animation-name: agent_" << a << "_anim;" << endl;
		svg << "    animation-duration: " << m_duration << "s;" << endl;
		svg << "    animation-iteration-count: infinite;" << endl;
		svg << "    animation-timing-function: linear;" << endl;
		svg << "  }" << endl;
	}
	svg << "</style>" << endl;

	for(unsigned a=0; a<input.agent_count(); ++a)
		svg << "<circle id=\"agent_" << a << "\" r=\"" << m_radius << "\" "
			//"style=\"" << m_style << "\" "
			"/>" << endl;
}

const std::string animated_source_trajectory::type() {
	return "animated_source_trajectory";
}

}
