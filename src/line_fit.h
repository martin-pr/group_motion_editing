#pragma once

#include <vector>

#include <ImathVec.h>

struct line {
	Imath::Vec2<float> origin = Imath::Vec2<float>(0,0);
	Imath::Vec2<float> direction = Imath::Vec2<float>(0,0);
};

line fit(const std::vector<Imath::Vec2<float>>& points);
