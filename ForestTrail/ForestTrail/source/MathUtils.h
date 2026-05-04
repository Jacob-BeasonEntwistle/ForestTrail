#pragma once
#include <cstdlib>
#include <glm/glm.hpp>

inline float randomRange(float min, float max) {
	// rand gives int between 0 and RAND_MAX
	// Dividing by RAND_MAX normalizes it to [0.0, 1.0]
	// Multiply it by the range size (e.g. (-30) - (-50) = 20)
	// Then add the minimum to shift it into the correct range
	return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}

inline glm::vec3 randomVec3(
	float minX, float maxX,
	float minY, float maxY,
	float minZ, float maxZ
) {
	float x = minX + static_cast<float>(rand()) / RAND_MAX * (maxX - minX);
	float y = minY + static_cast<float>(rand()) / RAND_MAX * (maxY - minY);
	float z = minZ + static_cast<float>(rand()) / RAND_MAX * (maxZ - minZ);
	
	return glm::vec3(x, y, z);
}
