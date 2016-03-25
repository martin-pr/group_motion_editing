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

void animated_source_trajectory::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const {
	const unsigned id = uid();

	svg << "<style type=\"text/css\">" << endl;
	for(unsigned a=0; a<input.agent_count(); ++a) {
		svg << "  @keyframes agent_" << a << "_anim_" << id << " {" << endl;
		for(unsigned f=0; f<input.frame_count(); ++f) {
			const float t = (float)f / (float)(input.frame_count()-1) * 100;
			// svg << "    " << t << "% { left:" << input[a][f].position.x << "px; top: " << input[a][f].position.y << "px; }" << endl;
			svg << "    " << t << "% { transform: translateX(" << input[a][f].position.x << "px) translateY(" << input[a][f].position.y << "px); }" << endl;
		}
		svg << "  }" << endl;

		svg << "  #agent_" << a << "_" << id << " {" << endl;
		svg << "    animation-name: agent_" << a << "_anim_" << id << ";" << endl;
		svg << "    animation-duration: " << m_duration << "s;" << endl;
		svg << "    animation-iteration-count: infinite;" << endl;
		svg << "    animation-timing-function: linear;" << endl;
		svg << "  }" << endl;
	}
	svg << "</style>" << endl;

	for(unsigned a=0; a<input.agent_count(); ++a)
		svg << "<circle id=\"agent_" << a << "_" << id << "\" r=\"" << m_radius << "\" "
			//"style=\"" << m_style << "\" "
			"/>" << endl;
}

const std::string animated_source_trajectory::type() {
	return "animated_source_trajectory";
}

}
