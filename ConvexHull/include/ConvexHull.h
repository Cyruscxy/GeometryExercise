#ifndef CONVEX_HULL_H
#define CONVEX_HULL_H

#include <vector>
#include "math/vec.h"

namespace ConvexHull
{
	void genRandomVertices(std::vector<Vec3>& vertices);

	void genConvexHull(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& edges);
}

#endif