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

		// if there is a division by zero, we've hit precisely a point in the source curve
		//   -> just return its value and be done
		if(std::isinf(weight))
			return p.second;

		norm += weight;
		value += p.second * weight;
	}

	return value / norm;
}

agents shepards::apply(const agents& source) const {
	// get the heading line / direction
	line l = source.heading();
	// normalize the direction vector - we'll need to use it to compute acos
	l.direction.normalize();
	// and compute the angle to the world axes
	const float lAngle = atan2(l.direction.y, l.direction.x);

	agents result(source);

	for(unsigned agentId = 0; agentId < source.agent_count(); ++agentId) {
		auto& agent = source[agentId];

		// first frame's positions should be adjusted to the local axes
		{
			// compute the angular difference between line's direction and curve's beginning
			const Imath::Vec2<float> s = m_leading_curve.normdiff(0.0f);
			// and compute its angle to world axes
			const float sAngle = atan2(s.y, s.x);
			// the full angle is the difference
			const float angle = sAngle - lAngle;

			const float cs = cos(angle);
			const float sn = sin(angle);

			// make a vector from line origin to trajectory start
			const Imath::Vec2<float> d = agent[0].position - l.origin;

			// rotate it to match local coord system, relative to leading curve's start point
			result[agentId][0].position = m_leading_curve[0] + Imath::Vec2<float>(
				d.x * cs - d.y * sn,
				d.y * cs + d.x * sn
			);

			// and rotate direction in the same manner
			const Imath::Vec2<float>& dir = agent[0].direction;
			result[agentId][0].direction = Imath::Vec2<float>(
				dir.x * cs - dir.y * sn,
				dir.y * cs + dir.x * sn
			);
		}

		for(unsigned frameId = 0; frameId < agent.size(); ++frameId) {
			// evaluate the shepard's function at a local point
			const Imath::Vec2<float> s = sample(result[agentId][std::max((int)frameId-1, 0)].position);
			// and compute its angle to world axes
			const float sAngle = atan2(s.y, s.x);
			// the full angle is the difference
			const float angle = sAngle - lAngle;

			const float cs = cos(angle);
			const float sn = sin(angle);

			// update the position, except first frame (there we keep the original)
			if(frameId > 0) {
				Imath::Vec2<float> diff = agent[frameId].position - agent[frameId-1].position;

				result[agentId][frameId].position = result[agentId][frameId-1].position + Imath::Vec2<float>(
					diff.x * cs - diff.y * sn,
					diff.y * cs + diff.x * sn
				);
			}

			// and update the direction
			auto& dir = result[agentId][frameId].direction;
			dir = Imath::Vec2<float>(
				dir.x * cs - dir.y * sn,
				dir.y * cs + dir.x * sn
			);
		}
	}

	return result;
}

}
