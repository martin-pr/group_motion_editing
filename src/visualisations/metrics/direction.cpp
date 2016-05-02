#include "direction.h"

#include <iomanip>

#include "scenarios/shepards.h"
#include "../../line_fit.h"

using std::cout;
using std::endl;

namespace visualisations { namespace metrics {

direction::direction(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_ramp = colour_ramp(config.get_child("ramp"));
	m_radius = config.get("r", 5.0f);

	if(m_ramp.empty())
		throw std::runtime_error("An empty ramp found in direction visualisation");
}

direction::~direction() {
}

void direction::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	if((output.agent_count() > 1) && (!m_ramp.empty())) {
		// check that all the trajectories have the same number of samples (they should)
		const std::size_t sampleCount = output.frame_count();
		for(auto& as : output)
			if(as.size() != sampleCount)
				throw std::runtime_error("Non-consistent trajectory samples in output agent array in direction visualisation");

		// based on the directions, draw a circle with the right colour
		for(unsigned a=0;a<output.agent_count();++a) {
			for(unsigned i=1; i<sampleCount-1; ++i) {
				// this will probably need fixing for PI / -PI flip problems

				const Imath::V2f d1i = (input[a][i].position - input[a][i-1].position).normalized();
				const Imath::V2f d2i = (input[a][i+1].position - input[a][i].position).normalized();
				const Imath::V2f d1o = (output[a][i].position - output[a][i-1].position).normalized();
				const Imath::V2f d2o = (output[a][i+1].position - output[a][i].position).normalized();

				const float a1i = atan2(d1i.y, d1i.x);
				const float a2i = atan2(d2i.y, d2i.x);
				const float a1o = atan2(d1o.y, d1o.x);
				const float a2o = atan2(d2o.y, d2o.x);

				const float ai = (a2i - a1i) / M_PI * 180.0f;
				const float ao = (a2o - a1o) / M_PI * 180.0f;

				const float value = fabs(ai - ao);

				svg << "<circle cx=\"" << output[a][i].position.x << "\" "
						"cy=\"" << output[a][i].position.y << "\" "
						"r=\"" << m_radius << "\" "
						<< m_ramp.fill(value) << " "
						"style=\"" << m_style << "\" />" << std::endl;
			}
		}
	}
}

const std::string direction::type() {
	return "metrics/direction";
}

} }

