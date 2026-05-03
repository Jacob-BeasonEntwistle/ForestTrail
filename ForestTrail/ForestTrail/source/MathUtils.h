#pragma once
#include <cstdlib>

inline float randomRange(float min, float max) {
	// rand gives int between 0 and RAND_MAX
	// Dividing by RAND_MAX normalizes it to [0.0, 1.0]
	// Multiply it by the range size (e.g. (-30) - (-50) = 20)
	// Then add the minimum to shift it into the correct range
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}
