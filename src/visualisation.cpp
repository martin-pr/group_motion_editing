#include "visualisation.h"

visualisation::visualisation(const boost::property_tree::ptree& config) {
	for(auto& vis : config) {
		const std::string type = vis.second.get_child("type").data();
		std::unique_ptr<visualisations::base> instance =
			factory<visualisations::base,boost::property_tree::ptree>::singleton().
				create(type, vis.second);

		m_visualisations.push_back(std::move(instance));
	}
}
