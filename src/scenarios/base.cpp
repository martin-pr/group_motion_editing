#include "base.h"

using std::cout;
using std::endl;

namespace scenarios {

base::base(const boost::property_tree::ptree& config) {
	for(auto& vis : config.get_child("visualisations"))
		m_visualisations.insert(std::make_pair(vis.first, visualisation(vis.second)));
}

}
