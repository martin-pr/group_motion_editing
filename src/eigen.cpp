#include "eigen.h"

#include <cmath>

std::array<eigen_result<2>, 2> eigen<2>::compute(const std::array<std::array<float, 2>, 2>& m) {
	std::array<eigen_result<2>, 2> result;

	// based on http://www.math.harvard.edu/archive/21b_fall_04/exhibits/2dmatrices/index.html
	const float t = m[0][0] + m[1][1];
	const float d = m[0][0]*m[1][1] - m[1][0]*m[0][1];

	result[0].value = t/2.0f + sqrt(t*t/4.0f-d);
	result[1].value = t/2.0f - sqrt(t*t/4.0f-d);

	// m[1][0] != 0 (its abs value has to be strictly larger than b - there are two complementary
	//   ways of computing eigenvectors, the highest abs value gives more accuracy)
	if(abs(m[1][0]) > abs(m[0][1])) {
		result[0].vector = std::array<float,2>{result[0].value - m[1][1], m[1][0]};
		result[1].vector = std::array<float,2>{result[1].value - m[1][1], m[1][0]};
	}
	// m[0][1] != 0 (could do the same test as above = only m[0][1] == m[1][0] == 0.0f is an edge case)
	else if(m[0][1] != 0.0f) {
		result[0].vector = std::array<float,2>{m[0][1], result[0].value - m[0][0]};
		result[1].vector = std::array<float,2>{m[0][1], result[1].value - m[0][0]};
	}
	// both m[0][1] and m[1][0] are zero = orthonormal basis corresponds to the world coord system
	else {
		result[0].vector = std::array<float,2>{1,0};
		result[1].vector = std::array<float,2>{0,1};
	}

	return result;
}
