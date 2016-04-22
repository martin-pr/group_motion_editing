#pragma once

#include <ImathVec.h>

#include "base.h"
#include "curve.h"
#include "../bezier.h"

namespace scenarios {

class shepards : public scenario<shepards>, public curve {
	public:
		static std::string type();

		shepards(const boost::property_tree::ptree& config);

		float sampling() const;

		Imath::Vec2<float> sample(const Imath::Vec2<float>& pos) const;

		virtual agents apply(const agents& source) const override;

	protected:
	private:
		float m_sampling, m_power;
		unsigned m_stepCount;

		std::vector<std::pair<Imath::Vec2<float>, Imath::Vec2<float>>> m_samples;
};

}
