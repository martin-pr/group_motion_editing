#pragma once

#include <ImathVec.h>

#include "base.h"
#include "../bezier.h"

namespace scenarios {

class shepards : public scenario<shepards> {
	public:
		static std::string type();

		shepards(const boost::property_tree::ptree& config);

		const bezier<Imath::Vec2<float>>& leading_curve() const;
		float sampling() const;

		Imath::Vec2<float> sample(const Imath::Vec2<float>& pos) const;

		virtual agents apply(const agents& source) const override;

	protected:
	private:
		bezier<Imath::Vec2<float>> m_leading_curve;
		float m_sampling, m_power;
		unsigned m_stepCount;

		std::vector<std::pair<Imath::Vec2<float>, Imath::Vec2<float>>> m_samples;
};

}
