#include "distance.h"

#include <iomanip>

#include "scenarios/shepards.h"
#include "../../line_fit.h"

using std::cout;
using std::endl;

namespace visualisations { namespace metrics {

distance::distance(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_ramp = colour_ramp(config.get_child("ramp"));

	if(m_ramp.empty())
		throw std::runtime_error("An empty ramp found in distance visualisation");
}

distance::~distance() {
}

void distance::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	if((output.agent_count() > 1) && (!m_ramp.empty())) {
		// check that all the trajectories have the same number of samples (they should)
		const std::size_t sampleCount = output.frame_count();
		for(auto& as : output)
			if(as.size() != sampleCount)
				throw std::runtime_error("Non-consistent trajectory samples in output agent array in distance visualisation");

		// based on the distances, draw a line with the right colour
		for(unsigned i=0; i<sampleCount; ++i) {
			for(unsigned a=0;a<output.agent_count();++a)
				for(unsigned b=a+1;b<output.agent_count();++b) {
					const Imath::Vec2<float> p1 = output[a][i].position;
					const Imath::Vec2<float> p2 = output[b][i].position;
					const float dist = (p1 - p2).length();

					svg << "<path d=\"M" << p1.x << "," << p1.y << " L" << p2.x << "," << p2.y << "\" "
						<< "style=\"" << m_style << "\" "
						<< m_ramp.stroke(dist) << " />" << endl;
				}
		}
	}
}

const std::string distance::type() {
	return "metrics/distance";
}

} }

