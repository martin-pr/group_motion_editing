#pragma once

#include "base.h"

#include "../bezier.h"

namespace scenarios {

class curve : public boost::noncopyable {
	public:
		curve(const boost::property_tree::ptree& config);

		const bezier<Imath::Vec2<float>>& leading_curve() const;

	protected:
	private:
		bezier<Imath::Vec2<float>> m_leading_curve;
};

}
