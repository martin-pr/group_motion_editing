#include "vector_field.h"

#include "scenarios/shepards.h"

namespace visualisations {

vector_field::vector_field(const boost::property_tree::ptree& config) {
	m_style = config.get("style", std::string(""));

	m_sampling = config.get("sampling", 10u);

	std::vector<float> bbox;
	for(auto& p : config.get_child("bbox"))
		bbox.push_back(p.second.get<float>(""));
	if(bbox.size() != 4)
		throw std::runtime_error("vector_field - invalid number of bbox vertices");
	m_bbox.min = Imath::V2f(bbox[0], bbox[1]);
	m_bbox.max = Imath::V2f(bbox[2], bbox[3]);
}

vector_field::~vector_field() {
}

void vector_field::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& _scenario) const {
	const unsigned id = uid();

	const scenarios::shepards& scenario = dynamic_cast<const scenarios::shepards&>(_scenario);

	svg << "<defs>"
    			"<marker id=\"field_arrow_" << id << "\" markerWidth=\"5\" markerHeight=\"5\" refX=\"1.5\" refY=\"1.5\" orient=\"auto\" markerUnits=\"strokeWidth\">"
      				"<path d=\"M0,0 L0,3 L4.5,1.5 z\" style=\"" << m_style << "\" />"
    			"</marker>"
  			"</defs>" << std::endl;

	const unsigned x_sample_count = round((m_bbox.size().x - (float)m_sampling * 0.2f) / (float)m_sampling);
	const unsigned y_sample_count = round((m_bbox.size().y - (float)m_sampling * 0.2f) / (float)m_sampling);

	for(unsigned xi=0; xi<x_sample_count; ++xi)
		for(unsigned yi=0; yi<y_sample_count; ++yi) {
			const Imath::V2f pos(xi * m_sampling + m_sampling/2, yi * m_sampling + m_sampling/2);
			const Imath::V2f val = scenario.sample(pos) * (float)m_sampling * 0.4f;

			svg << "<line "
				"x1=\"" << (pos.x - val.x) << "\" "
				"y1=\"" << (pos.y - val.y) << "\" "
				"x2=\"" << (pos.x + val.x) << "\" "
				"y2=\"" << (pos.y + val.y) << "\" "
				"style=\"" << m_style << "\" "
				"marker-end=\"url(#field_arrow_" << id << ")\" "
				" />" << std::endl;
		}
}

const std::string vector_field::type() {
	return "vector_field";
}

}
