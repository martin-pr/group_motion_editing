#pragma once

#include <vector>

#include <ImathVec.h>

/// a simple container for agent frames.
/// Intentionally doesn't include resize methods - it should not be possible to change
/// individual frame counts in trajectory references returned from agents class.
class trajectory {
	public:
		struct frame {
			Imath::Vec2<float> position, direction;

			frame operator*(const float& w) const;
			frame operator+(const frame& fr) const;
		};

		trajectory(unsigned size);
		~trajectory();

		/// overloaded assignment operator, making sure that the number of frames
		///   matches the current frame count. Avoids resizing errors in agents class.
		trajectory& operator = (const trajectory& tr);

		std::size_t size() const;

		/// returns a reference to a frame with given index.
		const frame& operator[](unsigned index) const;
		/// returns a reference to a frame with given index (editable).
		frame& operator[](unsigned index);

		/// returns an interpolated frame (not a reference - edits don't change the curve)
		frame interpolated(float t) const;

		typedef std::vector<frame>::const_iterator const_iterator;
		const_iterator begin() const;
		const_iterator end() const;

		typedef std::vector<frame>::iterator iterator;
		iterator begin();
		iterator end();

	protected:
	private:
		std::vector<frame> m_frames;
};
