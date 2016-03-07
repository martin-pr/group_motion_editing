#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {

class animated_trajectory : public visualisation<animated_trajectory> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		animated_trajectory(const boost::property_tree::ptree& config);
		virtual ~animated_trajectory();

		virtual void draw(std::ostream& svg, const agents& input, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		float m_radius, m_duration;
};

}
