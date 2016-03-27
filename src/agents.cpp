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

line agents::heading() const {
	assert(agent_count() > 0 && frame_count() > 0);

	// collect all points
	std::vector<Imath::Vec2<float>> points;
	for(auto& a : m_agents)
		for(auto& f : a)
			points.push_back(f.position);

	// fit the line into points
	line l = fit(points);
	l.direction.normalize();

	// make sure it aims the right way, based on the average direction of agents in the first frame
	{
		Imath::Vec2<float> firstFrameDir = Imath::Vec2<float>(0,0);
		for(auto& a : m_agents)
			firstFrameDir += a.begin()->direction.normalized();

		if(l.direction.dot(firstFrameDir) < 0.0f)
			l.direction = -l.direction;
	}

	// figure out minimum and maximum parameter (t)
	float minT = 0, maxT = 0;
	for(auto& a : m_agents)
		for(auto& f : a) {
			const float t = l.direction.dot(f.position - l.origin); // l.direction is normalized
			minT = std::min(t, minT);
			maxT = std::max(t, maxT);
		}

	// set the origin and direction
	l.origin += minT * l.direction;
	l.direction *= (maxT - minT);

	return l;
}
