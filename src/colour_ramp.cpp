#include "colour_ramp.h"

#include <iomanip>

#include <boost/lexical_cast.hpp>

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

colour_ramp::colour_ramp() {
}

colour_ramp::colour_ramp(const boost::property_tree::ptree& data) {
	for(auto& c : data)
		m_ramp.insert(std::make_pair(
			boost::lexical_cast<float>(c.first),
			Imath::C4f(
				c.second.get<float>("r"),
				c.second.get<float>("g"),
				c.second.get<float>("b"),
				c.second.get<float>("a", 1.0f)
			)
		));
}

bool colour_ramp::empty() const {
	return m_ramp.empty();
}

std::string colour_ramp::stroke(const float& val) const {
	const Imath::C4f col = colour(val);
	return "stroke=\"" + toRGB(col) + "\" stroke-opacity=\"" + boost::lexical_cast<std::string>(col.a) + "\"";
}

std::string colour_ramp::fill(const float& val) const {
	const Imath::C4f col = colour(val);
	return "fill=\"" + toRGB(col) + "\" fill-opacity=\"" + boost::lexical_cast<std::string>(col.a) + "\"";
}

Imath::C4f colour_ramp::colour(const float& val) const {
	Imath::C4f color;
	if(val <= m_ramp.begin()->first)
		color = m_ramp.begin()->second;
	else if(val >= m_ramp.rbegin()->first)
		color = m_ramp.rbegin()->second;
	else {
		auto it1 = m_ramp.lower_bound(val);
		auto it2 = it1--;

		assert(it1 != m_ramp.end());
		assert(it2 != m_ramp.end());
		assert(it1->first < it2->first);

		const float w = (val - it1->first) / (it2->first - it1->first);
		color = it1->second * (1.0f - w) + it2->second * w;
	}

	return color;
}
