#include "projection_lines.h"

#include "scenarios/projection.h"

using std::cout;
using std::endl;

namespace visualisations {

projection_lines::projection_lines(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_length = config.get("length", 20.0f);
	m_sample_count = 1.0f / config.get("sampling", 0.01f);
}

projection_lines::~projection_lines() {
}

namespace {
	Imath::Vec2<float> project(const bezier<Imath::Vec2<float>>& curve, const float& param, const float& dist) {
		auto p = curve[param];
		auto d = curve.normdiff(param);

		return p + Imath::Vec2<float>(-d.y, d.x)*dist;
	}
}

void projection_lines::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	svg << "<path style=\"" << m_style << "\" d=\"";

	// get the original scenario
	const scenarios::projection& scenario = dynamic_cast<const scenarios::projection&>(_scenario);
	// and get the leading curve
	const bezier<Imath::Vec2<float>> c = scenario.leading_curve();

	// draw the lines
	for(unsigned s=0;s<m_sample_count;++s) {
		const float param = (float)s / (float)(m_sample_count-1);

		const Imath::Vec2<float> p1 = project(c, param, -m_length);
		const Imath::Vec2<float> p2 = project(c, param, m_length);

		svg << "M" << p1.x << "," << p1.y << " L" << p2.x << "," << p2.y << " ";
	}

	svg << "\" />" << std::endl;
}

const std::string projection_lines::type() {
	return "projection_lines";
}

}

