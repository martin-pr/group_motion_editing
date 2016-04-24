#pragma once

#include <boost/property_tree/ptree.hpp>

#include <ImathColor.h>

#include "base.h"

namespace visualisations {

class distance : public visualisation<distance> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		distance(const boost::property_tree::ptree& config);
		virtual ~distance();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		std::map<float, Imath::C4f> m_ramp;
};

}
