#pragma once

#include <array>

template<unsigned DIM>
struct eigen_result {
	float value;
	std::array<float, DIM> vector;
};

template<unsigned DIM>
struct eigen {
	/// generic version not implemented - not needed at the mo
	static std::array<eigen_result<DIM>, DIM> compute(const std::array<std::array<float, DIM>, DIM>& m);
};

template<>
struct eigen<2> {
	static std::array<eigen_result<2>, 2> compute(const std::array<std::array<float, 2>, 2>& m);
};
