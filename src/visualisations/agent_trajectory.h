#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {

class agent_trajectory : public visualisation<agent_trajectory> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		agent_trajectory(const boost::property_tree::ptree& config);
		virtual ~agent_trajectory();

		virtual void draw(std::ostream& svg, const agents& input, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style, m_render;
		float m_radius;
};

}
