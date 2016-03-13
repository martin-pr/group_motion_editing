#include "visualisation.h"

#include "visualisations/base.h"
#include "factory.h"

visualisation::visualisation(const boost::property_tree::ptree& config) {
	for(auto& vis : config) {
		const std::string type = vis.second.get_child("type").data();
		std::unique_ptr<visualisations::base> instance(
			factory<visualisations::base,boost::property_tree::ptree>::singleton().
				create(type, vis.second));

		m_visualisations.push_back(std::move(instance));
	}
}

visualisation::~visualisation() {

}

void visualisation::draw(std::ostream& svg, const agents& input, const agents& output, const scenarios::base& scenario) const {
	for(auto& vis : m_visualisations)
		vis->draw(svg, input, output, scenario);
}
