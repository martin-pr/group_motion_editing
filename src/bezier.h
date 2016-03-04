#pragma once

#include <vector>
#include <cassert>

template<typename T>
class bezier {
	public:
		void add_point(const T& p);
		T operator[](const float& param) const;

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
		for(unsigned a = m_points.size()-1; a > 0; --a)
			for(unsigned b = 0; b < a; ++b)
				tmp[b] = tmp[b] * (1.0f - param) + tmp[b+1] * param;

		return tmp.front();
	}
	else
		return m_points.front();
}
