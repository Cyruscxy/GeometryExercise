#include <iostream>
#include <string>
#include <polyscope/polyscope.h>
#include <polyscope/curve_network.h>
#include "ConvexHull.h"
#include "imgui.h"

std::vector<Vec3> vertices(100);
std::vector<std::vector<uint32_t>> edges;

void redrawVertices()
{
	ConvexHull::genRandomVertices(vertices);
	edges.clear();
	polyscope::removeCurveNetwork("Convex Hull");
	polyscope::registerCurveNetwork("Convex Hull", vertices, edges);
	polyscope::requestRedraw();
}

void drawEdges()
{
	edges.clear();
	ConvexHull::genConvexHull(vertices, edges);
	polyscope::removeCurveNetwork("Convex Hull");
	polyscope::registerCurveNetwork("Convex Hull", vertices, edges);
	polyscope::requestRedraw();
}

void functionCallback()
{
	if ( ImGui::Button("Gen Vertices") )
	{
		edges.clear();
		redrawVertices();
	}

	if ( ImGui::Button("Cal Convex Hull") )
	{
		drawEdges();
	}
}

int main() {

	
	ConvexHull::genRandomVertices(vertices);
	ConvexHull::genConvexHull(vertices, edges);


	polyscope::init();

	polyscope::state::userCallback = functionCallback;

	auto mesh = polyscope::registerCurveNetwork("Convex Hull", vertices, edges);

	polyscope::show();

    return 0;
}