#include "leading_curve.h"

#include "scenarios/shepards.h"

namespace visualisations {

leading_curve::leading_curve(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
}

leading_curve::~leading_curve() {

}

void leading_curve::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	const scenarios::shepards& scenario = dynamic_cast<const scenarios::shepards&>(_scenario);

	svg << "<polyline points=\"";
	const unsigned sampling = (unsigned)round(1.0f / scenario.sampling());

	for(unsigned ti=0; ti<=sampling; ++ti) {
		const float t = (float)ti / (float)sampling;
		const Imath::Vec2<float> pos = scenario.leading_curve()[t];
		svg << pos.x << "," << pos.y << " ";
	}
	svg << "\" style=\"" << m_style << "\" />" << std::endl;
}

const std::string leading_curve::type() {
	return "leading_curve";
}

}
