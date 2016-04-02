#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {

class projection_curves : public visualisation<projection_curves> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		projection_curves(const boost::property_tree::ptree& config);
		virtual ~projection_curves();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		float m_distance;
		unsigned m_curve_count, m_sample_count;
};

}
