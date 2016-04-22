#include "projection_curves.h"

#include "scenarios/projection.h"

using std::cout;
using std::endl;

namespace visualisations {

projection_curves::projection_curves(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
	m_distance = config.get("distance", 20.0f);
	m_sample_count = 1.0f / config.get("sampling", 0.01f);
	m_curve_count = config.get("count", 10u);
}

projection_curves::~projection_curves() {
}

namespace {
	Imath::Vec2<float> project(const bezier<Imath::Vec2<float>>& curve, const float& param, const float& dist) {
		auto p = curve[param];
		auto d = curve.normdiff(param);

		return p + Imath::Vec2<float>(-d.y, d.x)*dist;
	}
}

void projection_curves::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	svg << "<path style=\"" << m_style << "\" d=\"";

	// get the original scenario
	const scenarios::projection& scenario = dynamic_cast<const scenarios::projection&>(_scenario);
	// and get the leading curve
	const bezier<Imath::Vec2<float>> c = scenario.leading_curve();

	// draw curves around the central one
	for(int ci = -(int)m_curve_count; ci <= (int)m_curve_count; ++ci) {
		if(ci != 0) {
			const float dist = (float)ci * m_distance;


			Imath::Vec2<float> p = project(c, 0.0f, dist);
			svg << "M" << p.x << "," << p.y << " ";
			for(unsigned a=1;a<m_sample_count;++a) {
				const float param = (float)a / (float)(m_sample_count-1);
				p = project(c, param, dist);
				svg << "L" << p.x << "," << p.y << " ";
			}
		}
	}

	svg << "\" />" << std::endl;
}

const std::string projection_curves::type() {
	return "projection_curves";
}

}

