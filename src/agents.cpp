#include "agents.h"

#include <cassert>

agents::agents(unsigned agentCount, unsigned frameCount) : m_agents(agentCount, trajectory(frameCount)) {
	assert((agentCount == 0) == (frameCount == 0));
}

agents::~agents() {
}

bool agents::empty() const {
	return m_agents.empty();
}

unsigned agents::agent_count() const {
	return m_agents.size();
}
unsigned agents::frame_count() const {
	if(empty())
		return 0;
	return m_agents[0].size();
}

trajectory& agents::operator[](unsigned agentId) {
	assert(agentId < m_agents.size());
	return m_agents[agentId];
}

const trajectory& agents::operator[](unsigned agentId) const {
	assert(agentId < m_agents.size());
	return m_agents[agentId];
}

agents::const_iterator agents::begin() const {
	return m_agents.begin();
}

agents::const_iterator agents::end() const {
	return m_agents.end();
}

agents::iterator agents::begin() {
	return m_agents.begin();
}

agents::iterator agents::end() {
	return m_agents.end();
}
