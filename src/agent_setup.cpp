#include "agent_setup.h"

agent_setup::agent_setup(const boost::property_tree::ptree& value) {
	for(auto& item : value) {
		const std::string type = item.second.get<std::string>("type");
		if(type != "curve")
			throw std::runtime_error("agent_setup - only curve type is supported at the moment.");

		agent a;

		a.sampling = item.second.get<float>("sampling");

		for(auto& point : item.second.get_child("points")) {
			std::vector<float> p;
			for(auto& pi : point.second)
				p.push_back(pi.second.get<float>(""));
			if(p.size() != 2)
				throw std::runtime_error("agent_setup - only 2D points are supported at the moment.");

			a.curve.add_point(Imath::Vec2<float>(p[0], p[1]));
		}

		m_agents.push_back(a);
	}
}

agent_setup::const_iterator agent_setup::begin() const {
	return m_agents.begin();
}

agent_setup::const_iterator agent_setup::end() const {
	return m_agents.end();
}

agent_setup::iterator agent_setup::begin() {
	return m_agents.begin();
}

agent_setup::iterator agent_setup::end() {
	return m_agents.end();
}
