#include "curve.h"

#include "../agents.h"

namespace scenarios {

curve::curve(const boost::property_tree::ptree& config) {
	for(auto& point : config.get_child("curve.points")) {
		std::vector<float> p;
		for(auto& pi : point.second)
			p.push_back(pi.second.get<float>(""));
		if(p.size() != 2)
			throw std::runtime_error("curve - only 2D points are supported at the moment.");

		m_leading_curve.add_point(Imath::Vec2<float>(p[0], p[1]));
	}
}

const bezier<Imath::Vec2<float>>& curve::leading_curve() const {
	return m_leading_curve;
}

line curve::heading(const agents& ag) {
	assert(ag.agent_count() > 0 && ag.frame_count() > 0);

	// collect all points
	std::vector<Imath::Vec2<float>> points;
	for(auto& a : ag)
		for(auto& f : a)
			points.push_back(f.position);

	// fit the line into points
	line l = fit(points);
	l.direction.normalize();

	// make sure it aims the right way, based on the average direction of agents in the first frame
	{
		Imath::Vec2<float> firstFrameDir = Imath::Vec2<float>(0,0);
		for(auto& a : ag)
			firstFrameDir += a.begin()->direction.normalized();

		if(l.direction.dot(firstFrameDir) < 0.0f)
			l.direction = -l.direction;
	}

	// figure out minimum and maximum parameter (t)
	float minT = 0, maxT = 0;
	for(auto& a : ag)
		for(auto& f : a) {
			const float t = l.direction.dot(f.position - l.origin); // l.direction is normalized
			minT = std::min(t, minT);
			maxT = std::max(t, maxT);
		}

	// set the origin and direction
	l.origin += minT * l.direction;
	l.direction *= (maxT - minT);

	return l;
}


}
