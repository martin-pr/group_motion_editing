#include "projection_naive.h"

using std::cout;
using std::endl;

namespace scenarios {

std::string projection_naive::type() {
	return "projection_naive";
}

projection_naive::projection_naive(const boost::property_tree::ptree& config) : scenario<projection_naive, projection>(config) {
}

agents projection_naive::apply(const agents& source) const {
	// first, fit a line into the data
	const line l = heading(source);

	// normalize the direction vector - we'll need to use it to compute acos
	const Imath::Vec2<float> direction = l.direction.normalized();
	// and compute the angle to the world axes
	const float lAngle = atan2(direction.y, direction.x);

	// resulting agents set
	agents result = source;

	for(auto& agent : result) {
		for(auto& frame : agent) {
			// todo: code replication between projection and shepards - should be moved to curve?

			// first, project the frame to the fitted line
			const float param = std::max(std::min(l.project(frame.position), 1.0f), 0.0f);

			// compute the angular difference between line's direction and curve's beginning
			const Imath::Vec2<float> s = leading_curve().normdiff(param);
			// and compute its angle to world axes
			const float sAngle = atan2(s.y, s.x);
			// the full angle is the difference
			const float angle = sAngle - lAngle;

			const float cs = cos(angle);
			const float sn = sin(angle);

			// make a vector from line origin to trajectory start
			const Imath::Vec2<float> d = frame.position - (l.origin+l.direction*param);

			// rotate it to match local coord system, relative to leading curve's start point
			frame.position = leading_curve()[param] + Imath::Vec2<float>(
				d.x * cs - d.y * sn,
				d.y * cs + d.x * sn
			);

			// and rotate direction in the same manner
			const Imath::Vec2<float>& dir = frame.direction;
			frame.direction = Imath::Vec2<float>(
				dir.x * cs - dir.y * sn,
				dir.y * cs + dir.x * sn
			);
		}
	}

	return result;
}

}
