#include "heading_direction.h"

#include "scenarios/shepards.h"
#include "../line_fit.h"

using std::cout;
using std::endl;

namespace visualisations {

heading_direction::heading_direction(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));
}

heading_direction::~heading_direction() {
}

void heading_direction::draw(std::ostream& svg, const agents& input, const scenarios::base& _scenario) const {
	assert(input.agent_count() > 0 && input.frame_count() > 0);

	// collect all points
	std::vector<Imath::Vec2<float>> points;
	for(auto& a : input)
		for(auto& f : a)
			points.push_back(f.position);

	// fit the line into points
	line l = fit(points);
	l.direction.normalize();

	// make sure it aims the right way, based on the average direction of agents in the first frame
	{
		Imath::Vec2<float> firstFrameDir;
		for(auto& a : input)
			firstFrameDir += a.begin()->direction.normalized();

		if(l.direction.dot(firstFrameDir) < 0.0f)
			l.direction = -l.direction;
	}

	// figure out minimum and maximum parameter (t)
	float minT = 0, maxT = 0;
	for(auto& a : input)
		for(auto& f : a) {
			const float t = l.direction.dot(f.position - l.origin); // l.direction is normalized
			minT = std::min(t, minT);
			maxT = std::max(t, maxT);
		}

	svg << "<defs>"
    			"<marker id=\"heading_arrow\" markerWidth=\"10\" markerHeight=\"10\" refX=\"3\" refY=\"3\" orient=\"auto\" markerUnits=\"strokeWidth\">"
      				"<path d=\"M0,0 L0,6 L9,3 z\" style=\"" << m_style << "\" />"
    			"</marker>"
  			"</defs>" << std::endl;

	svg << "<line "
		"x1=\"" << (l.origin + l.direction * minT).x << "\" "
		"y1=\"" << (l.origin + l.direction * minT).y << "\" "
		"x2=\"" << (l.origin + l.direction * maxT).x << "\" "
		"y2=\"" << (l.origin + l.direction * maxT).y << "\" "
		"style=\"" << m_style << "\" "
		"marker-end=\"url(#heading_arrow)\" "
		" />" << std::endl;
}

const std::string heading_direction::type() {
	return "heading_direction";
}

}

