#pragma once

#include <vector>

#include <ImathVec.h>

class agents {
	public:
		struct agent_frame {
			Imath::Vec2<float> position, direction;
		};

		class agent {
			public:
				agent_frame& operator[](unsigned frameId);
				const agent_frame& operator[](unsigned frameId) const;

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

		agent& operator[](unsigned agentId);
		const agent& operator[](unsigned agentId) const;

	protected:
	private:
		std::vector<agent> m_agents;
};

