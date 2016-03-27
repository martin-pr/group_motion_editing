#include "trajectory.h"

#include <cassert>

trajectory::trajectory(unsigned size) : m_frames(size) {
}

trajectory::~trajectory() {
}

trajectory& trajectory::operator = (const trajectory& tr) {
	assert(tr.size() == size());
	m_frames = tr.m_frames;
	return *this;
}

std::size_t trajectory::size() const {
	return m_frames.size();
}

const trajectory::frame& trajectory::operator[](unsigned index) const {
	assert(index < m_frames.size());
	return m_frames[index];
}

trajectory::frame& trajectory::operator[](unsigned index) {
	assert(index < m_frames.size());
	return m_frames[index];
}

trajectory::const_iterator trajectory::begin() const {
	return m_frames.begin();
}

trajectory::const_iterator trajectory::end() const {
	return m_frames.end();
}

trajectory::iterator trajectory::begin() {
	return m_frames.begin();
}

trajectory::iterator trajectory::end() {
	return m_frames.end();
}
