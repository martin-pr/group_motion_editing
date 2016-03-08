#pragma once

#include <boost/property_tree/ptree.hpp>

#include <ImathBox.h>

#include "base.h"

namespace visualisations {

class vector_field : public visualisation<vector_field> {
	public:
		/// required for the factory mechanism
		static const std::string type();

		vector_field(const boost::property_tree::ptree& config);
		virtual ~vector_field();

		virtual void draw(std::ostream& svg, const agents& input, const scenarios::base& scenario) const override;

	protected:
	private:
		std::string m_style;
		unsigned m_sampling;
		Imath::Box2f m_bbox;
};

}
