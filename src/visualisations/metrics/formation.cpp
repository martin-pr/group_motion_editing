#include "formation.h"

#include <iomanip>

#include "scenarios/shepards.h"
#include "../../line_fit.h"

using std::cout;
using std::endl;

namespace visualisations { namespace metrics {

formation::formation(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_ramp = colour_ramp(config.get_child("ramp"));

	if(m_ramp.empty())
		throw std::runtime_error("An empty ramp found in formation visualisation");
}

formation::~formation() {
}

void formation::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	if((output.agent_count() > 1) && (!m_ramp.empty())) {
		// check that all the trajectories have the same number of samples (they should)
		const std::size_t sampleCount = output.frame_count();
		for(auto& as : output)
			if(as.size() != sampleCount)
				throw std::runtime_error("Non-consistent trajectory samples in output agent array in formation visualisation");

		// based on the formations, draw a line with the right colour
		for(unsigned i=0; i<sampleCount; ++i) {
			for(unsigned a=0;a<output.agent_count();++a)
				for(unsigned b=a+1;b<output.agent_count();++b) {
					const Imath::Vec2<float> p1i = input[a][i].position;
					const Imath::Vec2<float> p2i = input[b][i].position;

					const Imath::Vec2<float> p1o = output[a][i].position;
					const Imath::Vec2<float> p2o = output[b][i].position;

					const float distIn = (p1i - p2i).length();
					const float distOut = (p1o - p2o).length();
					const float value = distIn > 0.0f ? distOut / distIn : 0.0f;

					svg << "<path d=\"M" << p1o.x << "," << p1o.y << " L" << p2o.x << "," << p2o.y << "\" "
						<< "style=\"" << m_style << "\" "
						<< m_ramp.stroke(value) << " />" << endl;
				}
		}
	}
}

const std::string formation::type() {
	return "metrics/formation";
}

} }

