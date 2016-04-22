#include "shepards.h"

using std::cout;
using std::endl;

namespace scenarios {

shepards::shepards(const boost::property_tree::ptree& config) : scenario<shepards>(config), curve(config) {
	m_sampling = config.get("curve.sampling", 0.05);
	m_power = config.get("power", -2);
	m_stepCount = config.get("step_count", 10u);

	const unsigned sample_count = (unsigned)round(1.0f / m_sampling);
	for(unsigned s=0; s<=sample_count; ++s) {
		const float t = (float)s / (float)sample_count;

		m_samples.push_back(std::make_pair(leading_curve()[t], leading_curve().normdiff(t)));
	}
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
	const line l = heading(source);
	// normalize the direction vector - we'll need to use it to compute acos
	const Imath::Vec2<float> direction = l.direction.normalized();
	// and compute the angle to the world axes
	const float lAngle = atan2(direction.y, direction.x);

	agents result(source);

	for(unsigned agentId = 0; agentId < source.agent_count(); ++agentId) {
		auto& agent = source[agentId];

		// first frame's positions should be adjusted to the local axes
		{
			// project the starting point of this agent onto the fitted curve
			//   (based on the fitting algorithm, this should lead to a value
			//   between 0 and 1)
			const float param = std::max(std::min(l.project(agent[0].position), 1.0f), 0.0f);

			// compute the angular difference between line's direction and curve's beginning
			const Imath::Vec2<float> s = leading_curve().normdiff(param);
			// and compute its angle to world axes
			const float sAngle = atan2(s.y, s.x);
			// the full angle is the difference
			const float angle = sAngle - lAngle;

			const float cs = cos(angle);
			const float sn = sin(angle);

			// make a vector from line origin to trajectory start
			const Imath::Vec2<float> d = agent[0].position - (l.origin+l.direction*param);

			// rotate it to match local coord system, relative to leading curve's start point
			result[agentId][0].position = leading_curve()[param] + Imath::Vec2<float>(
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

		// except first frame (there we keep the original)
		for(unsigned frameId = 1; frameId < agent.size(); ++frameId) {

			// update the position
			auto position = result[agentId][frameId-1].position;
			for(unsigned si=0;si<m_stepCount;++si) {
				// compute the difference between two samples (using trajectory interpolation)
				const float t1 = (float)(frameId-1) + (float)si / (float)m_stepCount;
				const float t2 = (float)(frameId-1) + (float)(si+1) / (float)m_stepCount;
				const Imath::Vec2<float> diff = agent.interpolated(t2).position - agent.interpolated(t1).position;

				// evaluate the shepard's function at a local point
				const Imath::Vec2<float> s = sample(position);
				// and compute its angle to world axes
				const float sAngle = atan2(s.y, s.x);
				// the full angle is the difference
				const float angle = sAngle - lAngle;

				const float cs = cos(angle);
				const float sn = sin(angle);

				// update the position
				position += Imath::Vec2<float>(
					diff.x * cs - diff.y * sn,
					diff.y * cs + diff.x * sn
				);
			}

			// and update the direction based on where the positional sample ended up
			Imath::Vec2<float> direction = agent[frameId].direction;

			// evaluate the shepard's function at a local point
			const Imath::Vec2<float> s = sample(position);
			// and compute its angle to world axes
			const float sAngle = atan2(s.y, s.x);
			// the full angle is the difference
			const float angle = sAngle - lAngle;

			const float cs = cos(angle);
			const float sn = sin(angle);

			direction = Imath::Vec2<float>(
				direction.x * cs - direction.y * sn,
				direction.y * cs + direction.x * sn
			);

			// save the new frame
			result[agentId][frameId] = trajectory::frame{position, direction};
		}
	}

	return result;
}

}
