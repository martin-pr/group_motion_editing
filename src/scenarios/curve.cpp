#include "curve.h"

namespace scenarios {

curve::curve(const boost::property_tree::ptree& config) {
	for(auto& point : config.get_child("curve.points")) {
		std::vector<float> p;
		for(auto& pi : point.second)
			p.push_back(pi.second.get<float>(""));
		if(p.size() != 2)
			throw std::runtime_error("shepards - only 2D points are supported at the moment.");

		m_leading_curve.add_point(Imath::Vec2<float>(p[0], p[1]));
	}
}

const bezier<Imath::Vec2<float>>& curve::leading_curve() const {
	return m_leading_curve;
}

}
