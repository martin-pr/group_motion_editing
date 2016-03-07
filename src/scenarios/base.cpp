#include "base.h"

#include "../visualisations/base.h"

using std::cout;
using std::endl;

namespace scenarios {

base::base(const boost::property_tree::ptree& config) {
	for(auto& vis : config.get_child("visualisations"))
		m_visualisations.insert(std::make_pair(
			vis.first,
			std::unique_ptr<visualisation>(new visualisation(vis.second))));
}

const std::map<std::string, std::unique_ptr<visualisation>>& base::visualisations() const {
	return m_visualisations;
}

}
