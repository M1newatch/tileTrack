
#include <iostream>
#include <list>
#include <cmath>
#include <vector>
#include <algorithm>

#include "maze_generator.h"

std::vector<MazeCoordinate> generateTurn(float centerX, float centerY, float radius, float startAngle, int segments) {
    return generateArc(centerX, centerY, radius, startAngle, startAngle + M_PI_2, segments);
}

void MazeGenerator::generateMaze()
{
    //outer polygon
    std::vector<MazeCoordinate> coords = {
        {6.0f, 11.0f},  // start
        {6.0f, 14.0f},
        {6.2f, 15.0f},
        {7.0f, 15.5f},
        {8.0f, 16.0f},
        {9.0f, 15.8f},
        {10.0f, 15.0f},
        {15.0f, 11.0f},
        {15.4f, 9.0f},
        {15.3f, 8.0f},
        {15.0f, 7.0f},
        {14.0f, 6.3f},
        {13.0f, 6.2f},
        {12.0f, 6.7f},
        {11.5f, 7.0f},
        {11.0f, 8.0f},
        {10.9f, 9.0f},
        {10.0f, 10.0f},
        {7.0f, 10.0f},
        {7.0f, 7.0f},
        {9.0f, 7.0f},
        {10.0f, 6.5f},
        {10.5f, 6.0f},
        {10.9f, 5.0f},
        {10.8f, 4.0f},
        {10.0f, 3.0f},
        {9.0f, 2.0f},
        {8.0f, 2.0f},
        {7.0f, 2.3f},
        {6.4f, 3.0f},
        {6.0f, 4.0f},
        {6.0f, 6.0f},
        {4.0f, 6.0f},
        {3.0f, 6.5f},
        {2.5f, 7.0f},
        {2.0f, 8.0f},
        {1.8f, 9.0f},
        {2.0f, 10.0f},
        {3.0f, 11.0f},
        {4.0f, 11.0f}   // end
    };

    Mazepolygon poly;
    for (const auto& coord : coords) {
        poly.coordinates.push_back(coord);
    }

    polygons.push_back(poly);

    //inner poly top right
    std::vector<MazeCoordinate> innerCoords1 = {
        {7.0f, 11.0f},  // start
        {7.0f, 14.0f},
        {7.5f, 14.5f},
        {8.0f, 15.0f},
        {9.0f, 14.7f},
        {9.6f, 14.0f},
        {14.0f, 9.3f},
        {14.2f, 9.0f},
        {14.2f, 8.0f},
        {14.0f, 7.8f},
        {13.0f, 7.4f},
        {12.0f, 8.0f},
        {12.0f, 9.0f},
        {11.6f, 10.0f},
        {11.0f, 10.5f},
        {10.0f, 11.0f}   // end
    };
     
    Mazepolygon innerPoly1;
    for (const auto& coord : innerCoords1) {
        innerPoly1.coordinates.push_back(coord);
    }

    polygons.push_back(innerPoly1);

    //inner poly middle left
    std::vector<MazeCoordinate> innerCoords2 = {
        {6.0f, 10.0f},  // start
        {4.0f, 10.0f},
        {3.5f, 9.5f},
        {2.9f, 9.0f},
        {3.0f, 8.0f},
        {4.0f, 7.1f},
        {5.0f, 7.0f},
        {6.0f, 7.0f}   // end
    };
    
    Mazepolygon innerPoly2;
    for (const auto& coord : innerCoords2) {
        innerPoly2.coordinates.push_back(coord);
    }

    polygons.push_back(innerPoly2);

    //inner poly bottom
    std::vector<MazeCoordinate> innerCoords3 = {
        {7.0f, 6.0f},  // start
        {9.0f, 5.9f},
        {9.7f, 5.0f},
        {9.6f, 4.0f},
        {9.0f, 3.3f},
        {8.4f, 3.1f},
        {8.0f, 3.2f},
        {7.1f, 4.0f},
        {7.0f, 5.0f}   // end
    };
    Mazepolygon innerPoly3;
    for (const auto& coord : innerCoords3) {
        innerPoly3.coordinates.push_back(coord);
    }

    polygons.push_back(innerPoly3);

    SolutionGraphNode node1;
    node1.coordinate = {6.5f, 10.5f};
    SolutionGraphNode node2;
    node2.coordinate = {6.5f, 14.5f};
    SolutionGraphNode node3;
    node3.coordinate = {5.5f, 15.5f};
    SolutionGraphNode node4;
    node4.coordinate = {14.5f, 9.5f};
    SolutionGraphNode node5;
    node5.coordinate = {14.5f, 7.5f};
    SolutionGraphNode node6;
    node6.coordinate = {12.0f, 7.0f};
    SolutionGraphNode node7;
    node7.coordinate = {11.0f, 10.0f};
    SolutionGraphNode node8;
    node8.coordinate = {10.0f, 10.5f};
    SolutionGraphNode node9;
    node9.coordinate = {3.0f, 10.3f};
    SolutionGraphNode node10;
    node10.coordinate = {3.4f, 8.5f};
    SolutionGraphNode node11;
    node11.coordinate = {4.5f, 6.5f};
    SolutionGraphNode node12;
    node12.coordinate = {9.5f, 6.5f};
    SolutionGraphNode node13;
    node13.coordinate = {10.5f, 4.5f};
    SolutionGraphNode node14;
    node14.coordinate = {8.5f, 2.5f};
    SolutionGraphNode node15;
    node15.coordinate = {6.5f, 3.5f};
    SolutionGraphNode node16;
    node16.coordinate = {6.5f, 10.0f};

}