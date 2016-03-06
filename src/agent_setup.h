#pragma once

#include <memory>

#include <boost/property_tree/ptree.hpp>

#include "agents.h"

/// essentially just a reader for agent setups.
/// The inputs are hardcoded for now - no need for factories as the number of possible inputs
/// will probably stay quite limited.
class agent_setup {
	public:
		agent_setup(const boost::property_tree::ptree& value);

		const agents& operator* () const;

	protected:
	private:
		std::unique_ptr<agents> m_agents;
};
