#include "shepards.h"

using std::cout;
using std::endl;

namespace scenarios {

shepards::shepards(const boost::property_tree::ptree& config) : scenario<shepards>(config) {
	assert(config.get<std::string>("type") == "shepards");

	for(auto& point : config.get_child("curve.points")) {
		std::vector<float> p;
		for(auto& pi : point.second)
			p.push_back(pi.second.get<float>(""));
		if(p.size() != 2)
			throw std::runtime_error("shepards - only 2D points are supported at the moment.");

		m_leading_curve.add_point(Imath::Vec2<float>(p[0], p[1]));
	}

	m_sampling = config.get("curve.sampling", 0.05);
	m_power = config.get("power", -2);

	const unsigned sample_count = (unsigned)round(1.0f / m_sampling);
	for(unsigned s=0; s<=sample_count; ++s) {
		const float t = (float)s / (float)sample_count;

		m_samples.push_back(std::make_pair(m_leading_curve[t], m_leading_curve.normdiff(t)));
	}
}

const bezier<Imath::Vec2<float>>& shepards::leading_curve() const {
	return m_leading_curve;
}

float shepards::sampling() const {
	return m_sampling;
}

std::string shepards::type() {
	return "shepards";
}

Imath::Vec2<float> shepards::sample(const Imath::Vec2<float>& pos) const {
	Imath::Vec2<float> value(0.0f, 0.0f);
	float norm = 0.0f;

	for(auto& p : m_samples) {
		const float weight = pow((pos - p.first).length(), m_power);

		norm += weight;
		value += p.second * weight;
	}

	return value / norm;
}

agents shepards::apply(const agents& source) const {
	return source;
}

}
