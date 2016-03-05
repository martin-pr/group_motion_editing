#pragma once

#include <boost/property_tree/ptree.hpp>

#include "visualisations/base.h"

class visualisation {
	public:
		visualisation(const boost::property_tree::ptree& config);

	protected:
	private:
		std::vector<std::unique_ptr<visualisations::base>> m_visualisations;
};
