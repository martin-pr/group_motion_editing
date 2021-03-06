#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {

class animated_source_trajectory : public visualisation<animated_source_trajectory> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		animated_source_trajectory(const boost::property_tree::ptree& config);
		virtual ~animated_source_trajectory();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		float m_radius, m_duration;
};

}
