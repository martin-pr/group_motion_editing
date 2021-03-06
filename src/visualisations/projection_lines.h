#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {

class projection_lines : public visualisation<projection_lines> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		projection_lines(const boost::property_tree::ptree& config);
		virtual ~projection_lines();

		virtual void draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		float m_length;
		unsigned m_sample_count;
};

}
