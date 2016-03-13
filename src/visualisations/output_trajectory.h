#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {

class output_trajectory : public visualisation<output_trajectory> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		output_trajectory(const boost::property_tree::ptree& config);
		virtual ~output_trajectory();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style, m_render;
		float m_radius;
};

}
