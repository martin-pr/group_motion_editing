#include "line_fit.h"

#include <cassert>

#include "eigen.h"

line fit(const std::vector<Imath::Vec2<float>>& points) {
	assert(points.size() >= 2);

	line result;

	for(auto& p : points)
		result.origin += p;
	result.origin /= points.size();

	float sx2 = 0, sy2 = 0, sxy = 0;
	for(auto& p : points) {
		sx2 += (p.x - result.origin.x) * (p.x - result.origin.x);
		sy2 += (p.y - result.origin.y) * (p.y - result.origin.y);
		sxy += (p.x - result.origin.x) * (p.y - result.origin.y);
	}

	const std::array<eigen_result<2>, 2> e = eigen<2>::compute(std::array<std::array<float,2>, 2>{
		std::array<float, 2> {
			sy2,
			-sxy
		},
		std::array<float, 2> {
			-sxy,
			sx2
		}
	});

	if(e[0].value > e[1].value)
		result.direction = Imath::Vec2<float>(e[1].vector[0], e[1].vector[1]);
	else
		result.direction = Imath::Vec2<float>(e[0].vector[0], e[0].vector[1]);

	return result;
}
