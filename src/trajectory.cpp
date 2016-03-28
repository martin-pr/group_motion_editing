#include "trajectory.h"

#include <cassert>

trajectory::frame trajectory::frame::operator*(const float& w) const {
	return trajectory::frame {
		position * w,
		direction * w
	};
}

trajectory::frame trajectory::frame::operator+(const frame& fr) const {
	return trajectory::frame {
		position + fr.position,
		direction + fr.direction
	};
}

/////////////

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

trajectory::frame trajectory::interpolated(float t) const {
	assert(t >= 0.0f && t <= (float)(size()-1));

	const unsigned first = floor(t);
	const unsigned second = ceil(t);

	// precise frame - return a reference to an existing frame
	if(first == second)
		return operator[](first);
	assert(first+1 == second);

	// interpolate between two frames - catmull-rom
	static const float alpha = 0.5f; // 0.0 ~ uniform spline, 0.5 ~ centripetal spline, 1.0 ~ chordal spline

	// weights for catmull rom (t0 is implicitly 0)
	float t1 = 0.0f;
	if(first > 0)
		t1 = pow((m_frames[first-1].position - m_frames[first].position).length(), alpha);
	const float t2 = t1 + pow((m_frames[first].position - m_frames[second].position).length(), alpha);
	float t3 = t2;
	if(second < size()-1)
		t3 = t2 + pow((m_frames[second].position - m_frames[second+1].position).length(), alpha);
	assert(t2 != t1);
	// t transformed into the new t0..t1..t2..t3 system
	const float tx = t1 + (t-floor(t))*(t2-t1);
	assert(0.0f <= t1 && t1 <= tx && tx <= t2 && t2 <= t3);

	// interpolation computation (can probably be done nicer via templates, but whatever)
	frame A1 = m_frames[first];
	if(t1 > 0.0f)
		A1 = m_frames[first-1]*((t1-tx)/t1) + m_frames[first]*(tx/t1);
	const frame A2 = m_frames[first]*((t2-tx)/(t2-t1)) + m_frames[second]*((tx-t1)/(t2-t1));
	frame A3 = m_frames[second];
	if(t2 < t3)
		A3 = m_frames[second]*((t3-tx)/(t3-t2)) + m_frames[second+1]*((tx-t2)/(t3-t2));

	frame B1 = A1*((t2-tx)/t2) + A2*(tx/t2);
	frame B2 = A2*((t3-tx)/(t3-t1)) + A3*((tx-t1)/(t3-t1));

	return B1*((t2-tx)/(t2-t1)) + B2*((tx-t1)/(t2-t1));
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
