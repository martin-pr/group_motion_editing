#pragma once

#include <boost/property_tree/ptree.hpp>

#include <ImathBox.h>

#include "base.h"

namespace visualisations {

class heading_direction : public visualisation<heading_direction> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		heading_direction(const boost::property_tree::ptree& config);
		virtual ~heading_direction();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
};

}
