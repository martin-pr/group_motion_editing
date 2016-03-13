#include "agents.h"

#include <cassert>

agents::agent_frame& agents::agent::operator[](unsigned frameId) {
	assert(frameId < m_frames.size());
	return m_frames[frameId];
}

const agents::agent_frame& agents::agent::operator[](unsigned frameId) const {
	assert(frameId < m_frames.size());
	return m_frames[frameId];
}

agents::agent::agent(unsigned frameCount) : m_frames(frameCount) {
}

agents::agent::const_iterator agents::agent::begin() const {
	return m_frames.begin();
}

agents::agent::const_iterator agents::agent::end() const {
	return m_frames.end();
}


///////////////


agents::agents(unsigned agentCount, unsigned frameCount) : m_agents(agentCount, agent(frameCount)) {
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
	return m_agents[0].m_frames.size();
}

agents::agent& agents::operator[](unsigned agentId) {
	assert(agentId < m_agents.size());
	return m_agents[agentId];
}

const agents::agent& agents::operator[](unsigned agentId) const {
	assert(agentId < m_agents.size());
	return m_agents[agentId];
}

agents::const_iterator agents::begin() const {
	return m_agents.begin();
}

agents::const_iterator agents::end() const {
	return m_agents.end();
}
