#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {

class leading_curve : public visualisation<leading_curve> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		leading_curve(const boost::property_tree::ptree& config);
		virtual ~leading_curve();

		virtual void draw(std::ostream& svg, const agents& input, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
};

}
