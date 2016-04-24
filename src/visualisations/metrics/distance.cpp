#include "distance.h"

#include <iomanip>

#include "scenarios/shepards.h"
#include "../../line_fit.h"

using std::cout;
using std::endl;

namespace visualisations { namespace metrics {

namespace {
	std::string toRGB(const Imath::C4f& col) {
		std::stringstream ss;
		ss << "#";

		for(unsigned a=0;a<3;++a) {
			int value = col[a] * 255.0f;
			value = std::max(0, value);
			value = std::min(255, value);

			ss << std::setw(2) << std::setfill('0') << std::setbase(16) << value;
		}

		return ss.str().c_str();
	}
}

distance::distance(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));

	for(auto& c : config.get_child("ramp"))
		m_ramp.insert(std::make_pair(
			boost::lexical_cast<float>(c.first),
			Imath::C4f(
				c.second.get<float>("r"),
				c.second.get<float>("g"),
				c.second.get<float>("b"),
				c.second.get<float>("a")
			)
		));

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

					Imath::C4f color;
					if(dist <= m_ramp.begin()->first)
						color = m_ramp.begin()->second;
					else if(dist >= m_ramp.rbegin()->first)
						color = m_ramp.rbegin()->second;
					else {
						auto it1 = m_ramp.lower_bound(dist);
						auto it2 = it1--;

						assert(it1 != m_ramp.end());
						assert(it2 != m_ramp.end());
						assert(it1->first < it2->first);

						const float w = (dist - it1->first) / (it2->first - it1->first);
						color = it1->second * (1.0f - w) + it2->second * w;
					}

					svg << "<path d=\"M" << p1.x << "," << p1.y << " L" << p2.x << "," << p2.y << "\" "
						<< "style=\"" << m_style << "\" "
						<< "stroke=\"" << toRGB(color) << "\" stroke-opacity=\"" << color.a << "\" />" << endl;
				}
		}
	}
}

const std::string distance::type() {
	return "metrics/distance";
}

} }

