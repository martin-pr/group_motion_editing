#pragma once

#include <vector>

#include <ImathVec.h>

#include "line_fit.h"
#include "trajectory.h"

class agents {
	public:
		agents(unsigned agentCount, unsigned frameCount);
		~agents();

		bool empty() const;
		unsigned agent_count() const;
		unsigned frame_count() const;

		/// computes the heading direction and origin (fits a line into the data,
		///   with start based on lowest edge point projection, and length based on
		///   the difference between projections of edge frames)
		line heading() const;

		trajectory& operator[](unsigned agentId);
		const trajectory& operator[](unsigned agentId) const;

		typedef std::vector<trajectory>::const_iterator const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

	protected:
	private:
		std::vector<trajectory> m_agents;
};

