#pragma once

#include <boost/property_tree/ptree.hpp>

#include "../base.h"
#include "../../colour_ramp.h"

namespace visualisations { namespace metrics {

class speed : public visualisation<speed> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		speed(const boost::property_tree::ptree& config);
		virtual ~speed();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		colour_ramp m_ramp;
};

} }
