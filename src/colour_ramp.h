#pragma once

#include <map>

#include <boost/property_tree/ptree.hpp>

#include <ImathColor.h>

class colour_ramp {
	public:
		colour_ramp();
		colour_ramp(const boost::property_tree::ptree& data);

		bool empty() const;

		std::string stroke(const float& val) const;

	protected:
		Imath::C4f colour(const float& val) const;

	private:
		std::map<float, Imath::C4f> m_ramp;
};
