#include "speed.h"

#include <iomanip>

#include "scenarios/shepards.h"
#include "../../line_fit.h"

using std::cout;
using std::endl;

namespace visualisations { namespace metrics {

speed::speed(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_ramp = colour_ramp(config.get_child("ramp"));

	if(m_ramp.empty())
		throw std::runtime_error("An empty ramp found in speed visualisation");
}

speed::~speed() {
}

void speed::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	if((output.agent_count() >= 1) && (!m_ramp.empty())) {
		assert(input.frame_count() == output.frame_count());
		assert(input.agent_count() == output.agent_count());

		// based on the speeds, draw a line with the right colour
		for(unsigned a=0;a<output.agent_count();++a) {
			for(unsigned fi=0; fi<input.frame_count()-1; ++fi) {
				const float inSpeed = (input[a][fi].position - input[a][fi+1].position).length();
				const float outSpeed = (output[a][fi].position - output[a][fi+1].position).length();
				const float speedRatio = (outSpeed > 0.0f) ? outSpeed / inSpeed : 0.0f;

				const Imath::Vec2<float> p1 = output[a][fi].position;
				const Imath::Vec2<float> p2 = output[a][fi+1].position;

				svg << "<path d=\"M" << p1.x << "," << p1.y << " L" << p2.x << "," << p2.y << "\" "
					<< "style=\"" << m_style << "\" "
					<< m_ramp.stroke(speedRatio) << " />" << endl;
			}
		}
	}
}

const std::string speed::type() {
	return "metrics/speed";
}

} }

