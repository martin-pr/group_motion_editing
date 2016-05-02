#pragma once

#include <boost/property_tree/ptree.hpp>

#include "../base.h"
#include "../../colour_ramp.h"

namespace visualisations { namespace metrics {

class direction : public visualisation<direction> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		direction(const boost::property_tree::ptree& config);
		virtual ~direction();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		colour_ramp m_ramp;
		float m_radius;
};

} }
