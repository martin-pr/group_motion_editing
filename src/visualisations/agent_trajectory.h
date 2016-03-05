#pragma once

#include <boost/property_tree/ptree.hpp>

#include "base.h"

namespace visualisations {
	
class agent_trajectory : public visualisation<agent_trajectory> {
	public:
		agent_trajectory(const boost::property_tree::ptree& config);
		virtual ~agent_trajectory();

		/// required for the factory mechanism
		static const std::string type();

	protected:
	private:
};

}
