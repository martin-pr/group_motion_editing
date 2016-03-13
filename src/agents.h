#pragma once

#include <vector>

#include <ImathVec.h>

#include "line_fit.h"

class agents {
	public:
		struct agent_frame {
			Imath::Vec2<float> position, direction;
		};

		class agent {
			public:
				agent_frame& operator[](unsigned frameId);
				const agent_frame& operator[](unsigned frameId) const;

				typedef std::vector<agent_frame>::const_iterator const_iterator;
				const_iterator begin() const;
				const_iterator end() const;

				std::size_t size() const;

			private:
				agent(unsigned frameCount);

				std::vector<agent_frame> m_frames;

			friend class agents;
		};

		agents(unsigned agentCount, unsigned frameCount);
		~agents();

		bool empty() const;
		unsigned agent_count() const;
		unsigned frame_count() const;

		/// computes the heading direction and origin (fits a line into the data,
		///   with start based on lowest edge point projection, and length based on
		///   the difference between projections of edge frames)
		line heading() const;

		agent& operator[](unsigned agentId);
		const agent& operator[](unsigned agentId) const;

		typedef std::vector<agent>::const_iterator const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

	protected:
	private:
		std::vector<agent> m_agents;
};

