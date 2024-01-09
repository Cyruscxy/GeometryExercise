#include "ConvexHull.h"
#include <random>
#include <glm/fwd.hpp>

void ConvexHull::genRandomVertices(std::vector<Vec3>& vertices)
{
	Real min = 0.0f;
	Real max = 1.0f;
	std::random_device seed;
	std::ranlux48 engine(seed());
	std::uniform_real_distribution<Real> distribution(min, max);

	for ( auto& vertex : vertices  )
	{
		Real theta = 2 * MathConst::PI * distribution(engine);
		Real r = distribution(engine);
		vertex.x = r * std::cos(theta);
		vertex.y = r * std::sin(theta);
		vertex.z = 0.0f;
	}
}

inline bool isLeftTurn(const Vec3& last, const Vec3& next)
{
	Real z = last.x * next.y - last.y * next.x;
	return z > 0.0f;
}

void ConvexHull::genConvexHull(std::vector<Vec3>& vertices, std::vector<std::vector<uint32_t>>& edges)
{
	if (vertices.size() <= 1) return;

	auto compareXCoord = [](const Vec3& lhs, const Vec3& rhs) -> bool
	{
		return lhs.x < rhs.x;
	};

	std::sort(vertices.begin(), vertices.end(), compareXCoord);

	std::vector<std::vector<uint32_t>> upperBound;
	uint32_t tail = 0;
	uint32_t head = 1;
	Vec3 lastVec = vertices[head] - vertices[tail];
	upperBound.push_back({ 0, 1 });

	// find upper bound
	for ( uint64_t i = 2; i < vertices.size(); ++i )
	{
		auto& next = vertices[i];
		Vec3 nextVec = next - vertices[head];
		while ( isLeftTurn(lastVec, nextVec) )
		{
			upperBound.pop_back();
			if (upperBound.empty() ) break;

			tail = upperBound.back()[0];
			head = upperBound.back()[1];
			lastVec = vertices[head] - vertices[tail];
			nextVec = next - vertices[head];
		}

		if (upperBound.empty()) upperBound.push_back({ 0, static_cast<uint32_t>(i) });
		else upperBound.push_back({ head, static_cast<uint32_t>(i) });

		tail = upperBound.back()[0];
		head = upperBound.back()[1];
		lastVec = vertices[head] - vertices[tail];
	}

	// find lower bound
	std::vector<std::vector<uint32_t>> lowerBound;
	tail = 0;
	head = 1;
	lastVec = vertices[head] - vertices[tail];
	lowerBound.push_back({ 0, 1 });
	for (uint64_t i = 2; i < vertices.size(); ++i)
	{
		auto& next = vertices[i];
		Vec3 nextVec = next - vertices[head];
		while (!isLeftTurn(lastVec, nextVec))
		{
			lowerBound.pop_back();
			if (lowerBound.empty()) break;

			tail = lowerBound.back()[0];
			head = lowerBound.back()[1];
			lastVec = vertices[head] - vertices[tail];
			nextVec = next - vertices[head];
		}

		if (lowerBound.empty()) lowerBound.push_back({ 0, static_cast<uint32_t>(i) });
		else lowerBound.push_back({ head, static_cast<uint32_t>(i) });

		tail = lowerBound.back()[0];
		head = lowerBound.back()[1];
		lastVec = vertices[head] - vertices[tail];
	}

	edges.insert(edges.end(), upperBound.begin(), upperBound.end());
	edges.insert(edges.end(), lowerBound.begin(), lowerBound.end());
}

