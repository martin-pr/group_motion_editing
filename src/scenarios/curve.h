#pragma once

#include "base.h"

#include "../bezier.h"
#include "../line_fit.h"

class agents;

namespace scenarios {

class curve : public boost::noncopyable {
	public:
		curve(const boost::property_tree::ptree& config);

		const bezier<Imath::Vec2<float>>& leading_curve() const;

		/// computes the heading direction and origin (fits a line into the data,
		///   with start based on lowest edge point projection, and length based on
		///   the difference between projections of edge frames)
		static line heading(const agents& a);

	protected:
	private:
		bezier<Imath::Vec2<float>> m_leading_curve;
};

}
