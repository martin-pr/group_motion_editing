#include "leading_curve.h"

#include "scenarios/curve.h"

namespace visualisations {

leading_curve::leading_curve(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_sample_count = 1.0f / config.get("sampling", 0.01f);
}

leading_curve::~leading_curve() {

}

void leading_curve::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	const scenarios::curve& scenario = dynamic_cast<const scenarios::curve&>(_scenario);

	svg << "<polyline points=\"";

	for(unsigned ti=0; ti<=m_sample_count; ++ti) {
		const float t = (float)ti / (float)m_sample_count;
		const Imath::Vec2<float> pos = scenario.leading_curve()[t];
		svg << pos.x << "," << pos.y << " ";
	}
	svg << "\" style=\"" << m_style << "\" />" << std::endl;
}

const std::string leading_curve::type() {
	return "leading_curve";
}

}
