#pragma once

#include <vector>

#include <boost/property_tree/ptree.hpp>

#include <ImathVec.h>

#include "bezier.h"

class agent_setup {
	public:
		struct agent {
			bezier<Imath::Vec2<float>> curve;
			float sampling;
		};

		agent_setup(const boost::property_tree::ptree& value);

		typedef std::vector<agent>::const_iterator const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

		typedef std::vector<agent>::iterator iterator;
		iterator begin();
		iterator end();

	protected:
	private:
		std::vector<agent> m_agents;
};
