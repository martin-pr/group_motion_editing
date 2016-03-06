#pragma once

#include <vector>
#include <cassert>

/// A generic representation of a bezier curve.
template<typename T>
class bezier {
	public:
		/// adds a point to the curve definitions.
		/// Each point raises the curve order by 1.
		void add_point(const T& p);

		/// returns a point on the curve (parameter in interval 0 to 1)
		T operator[](const float& param) const;
		/// returns a differential along the curve (parameter in interval 0 to 1)
		T normdiff(const float& param) const;

	protected:
	private:
		std::vector<T> m_points;
};

////////////////////////////////

template<typename T>
void bezier<T>::add_point(const T& p) {
	m_points.push_back(p);
}

template<typename T>
T bezier<T>::operator[](const float& param) const {
	assert(!m_points.empty());

	if(m_points.size() > 1)	{
		std::vector<T> tmp = m_points;

		// reduce the number of points to 1
		for(unsigned a = m_points.size()-1; a > 0; --a)
			for(unsigned b = 0; b < a; ++b)
				tmp[b] = tmp[b] * (1.0f - param) + tmp[b+1] * param;

		// and the remaining point is the result
		return tmp.front();
	}

	// only one point present - return it
	else
		return m_points.front();
}

template<typename T>
T bezier<T>::normdiff(const float& param) const {
	assert(!m_points.empty());

	if(m_points.size() > 1)	{
		std::vector<T> tmp = m_points;

		// reduce the number of points to 2
		for(unsigned a = m_points.size()-1; a > 1; --a)
			for(unsigned b = 0; b < a; ++b)
				tmp[b] = tmp[b] * (1.0f - param) + tmp[b+1] * param;

		// and return the differential between these points
		return (tmp[1] - tmp[0]).normalized();
	}
	// zero derivative
	else
		return T();
}
