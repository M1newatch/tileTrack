#pragma once
#include <list>

struct MazeCoordinate {
    float x;
    float y;
};

struct Mazepolygon {
    std::list<MazeCoordinate> coordinates;
};

struct SolutionGraphNode {
    MazeCoordinate coordinate;
    std::list<SolutionGraphNode*> neighbors;
};

class MazeGenerator {
public:
    std::list<Mazepolygon> polygons; // List of polygons that form the maze
    std::list<SolutionGraphNode> solutionGraph; // Graph representing the solution path through the maze
    void generateMaze();
}; 