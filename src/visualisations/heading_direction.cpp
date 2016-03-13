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

void heading_direction::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	const line l = input.heading();

	svg << "<defs>"
    			"<marker id=\"heading_arrow\" markerWidth=\"10\" markerHeight=\"10\" refX=\"3\" refY=\"3\" orient=\"auto\" markerUnits=\"strokeWidth\">"
      				"<path d=\"M0,0 L0,6 L9,3 z\" style=\"" << m_style << "\" />"
    			"</marker>"
  			"</defs>" << std::endl;

	svg << "<line "
		"x1=\"" << (l.origin).x << "\" "
		"y1=\"" << (l.origin).y << "\" "
		"x2=\"" << (l.origin + l.direction).x << "\" "
		"y2=\"" << (l.origin + l.direction).y << "\" "
		"style=\"" << m_style << "\" "
		"marker-end=\"url(#heading_arrow)\" "
		" />" << std::endl;
}

const std::string heading_direction::type() {
	return "heading_direction";
}

}

