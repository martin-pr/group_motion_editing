#pragma once

#include <vector>

#include <ImathVec.h>

struct line {
	Imath::Vec2<float> origin, direction;
};

line fit(const std::vector<Imath::Vec2<float>>& points);
