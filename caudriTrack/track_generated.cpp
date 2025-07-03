#include <iostream>
#include <list>
#include <cmath>
#include <vector>
#include <algorithm>

#include "maze_generator.h"

void MazeGenerator::generateMaze()
{
    // Polygon 1
    std::vector<MazeCoordinate> coords1 = {
        {4.9f, 7.2f},  // start
        {5.9f, 8.1f},
        {6.4f, 8.8f},
        {6.7f, 9.4f},
        {6.9f, 10.2f},
        {6.8f, 10.8f},
        {6.8f, 11.2f},
        {6.6f, 11.5f},
        {6.4f, 11.9f},
        {6.0f, 12.4f},
        {5.3f, 12.8f},
        {4.6f, 13.1f},
        {3.8f, 13.1f},
        {3.1f, 13.0f},
        {2.5f, 12.8f},
        {2.0f, 12.5f},
        {1.6f, 12.1f},
        {1.4f, 11.7f},
        {1.2f, 11.2f},
        {1.1f, 10.7f},
        {1.1f, 10.1f},
        {1.3f, 9.4f},
        {1.6f, 8.9f},
        {2.0f, 8.5f},
        {2.5f, 8.0f},
        {3.2f, 7.3f},
        {2.4f, 6.6f},
        {1.7f, 5.8f},
        {1.3f, 5.2f},
        {1.1f, 4.7f},
        {1.0f, 4.1f},
        {1.0f, 3.5f},
        {1.1f, 2.9f},
        {1.4f, 2.3f},
        {1.7f, 1.8f},
        {2.2f, 1.4f},
        {2.9f, 1.1f},
        {3.6f, 1.0f},
        {4.2f, 1.0f},
        {4.7f, 1.2f},
        {5.1f, 1.4f},
        {5.5f, 1.8f},
        {7.0f, 3.2f},
        {9.0f, 1.1f},
        {9.6f, 0.5f},
        {10.1f, 0.2f},
        {10.5f, 0.2f},
        {10.9f, 0.1f},
        {11.3f, 0.1f},
        {11.8f, 0.2f},
        {12.2f, 0.4f},
        {12.6f, 0.6f},
        {12.8f, 0.8f},
        {13.1f, 1.1f},
        {13.2f, 1.4f},
        {13.4f, 1.7f},
        {13.6f, 2.1f},
        {13.6f, 2.7f},
        {13.6f, 3.5f},
        {13.6f, 4.5f},
        {13.6f, 9.9f},
        {13.5f, 11.0f},
        {13.5f, 11.5f},
        {13.3f, 11.9f},
        {13.2f, 12.2f},
        {12.9f, 12.4f},
        {12.6f, 12.8f},
        {12.3f, 13.1f},
        {11.8f, 13.2f},
        {11.3f, 13.4f},
        {10.8f, 13.4f},
        {10.2f, 13.3f},
        {9.6f, 13.0f},
        {9.1f, 12.6f},
        {8.7f, 11.9f},
        {8.5f, 11.3f},
        {8.4f, 10.8f},
        {8.5f, 10.4f},
        {8.6f, 9.9f},
        {9.0f, 9.4f},
        {9.3f, 8.9f},
        {9.6f, 8.3f},
        {9.6f, 7.7f},
        {9.4f, 7.1f},
        {9.0f, 6.7f},
        {8.4f, 6.2f},
        {7.8f, 5.6f},
        {7.0f, 4.9f}   // end
    };

    Mazepolygon poly1;
    for (const auto& coord : coords1) {
        poly1.coordinates.push_back(coord);
    }

    polygons.push_back(poly1);

    // Polygon 2
    std::vector<MazeCoordinate> coords2 = {
        {4.0f, 8.1f},  // start
        {5.1f, 9.2f},
        {5.5f, 9.8f},
        {5.6f, 10.3f},
        {5.4f, 10.9f},
        {5.1f, 11.4f},
        {4.4f, 11.8f},
        {3.8f, 11.9f},
        {3.1f, 11.8f},
        {2.6f, 11.5f},
        {2.3f, 10.8f},
        {2.3f, 10.2f},
        {2.6f, 9.7f}   // end
    };

    Mazepolygon poly2;
    for (const auto& coord : coords2) {
        poly2.coordinates.push_back(coord);
    }

    polygons.push_back(poly2);

    // Polygon 3
    std::vector<MazeCoordinate> coords3 = {
        {4.0f, 6.4f},  // start
        {2.7f, 5.2f},
        {2.2f, 4.5f},
        {2.2f, 3.9f},
        {2.3f, 3.4f},
        {2.4f, 3.0f},
        {2.7f, 2.6f},
        {3.2f, 2.3f},
        {3.7f, 2.3f},
        {4.2f, 2.4f},
        {4.6f, 2.7f},
        {4.9f, 3.0f},
        {5.2f, 3.3f},
        {5.6f, 3.7f},
        {6.1f, 4.1f},
        {5.1f, 5.3f}   // end
    };

    Mazepolygon poly3;
    for (const auto& coord : coords3) {
        poly3.coordinates.push_back(coord);
    }

    polygons.push_back(poly3);

    // Polygon 4
    std::vector<MazeCoordinate> coords4 = {
        {7.9f, 4.1f},  // start
        {10.1f, 1.7f},
        {10.7f, 1.4f},
        {11.3f, 1.4f},
        {11.8f, 1.6f},
        {12.0f, 1.9f},
        {12.3f, 2.4f},
        {12.3f, 3.0f},
        {12.3f, 3.9f},
        {12.3f, 10.8f},
        {12.1f, 11.6f},
        {11.6f, 12.1f},
        {11.1f, 12.2f},
        {10.4f, 12.1f},
        {9.8f, 11.6f},
        {9.7f, 11.0f},
        {9.8f, 10.2f},
        {10.3f, 9.6f},
        {10.6f, 8.9f},
        {10.8f, 8.2f},
        {10.7f, 7.5f},
        {10.5f, 6.7f},
        {10.2f, 6.2f}   // end
    };

    Mazepolygon poly4;
    for (const auto& coord : coords4) {
        poly4.coordinates.push_back(coord);
    }

    polygons.push_back(poly4);

    // Solution path 1 nodes
    SolutionGraphNode node1;
    node1.coordinate = {7.4f, 3.7f};
    SolutionGraphNode node2;
    node2.coordinate = {7.5f, 3.6f};
    SolutionGraphNode node3;
    node3.coordinate = {7.8f, 3.2f};
    SolutionGraphNode node4;
    node4.coordinate = {8.2f, 2.8f};
    SolutionGraphNode node5;
    node5.coordinate = {8.6f, 2.3f};
    SolutionGraphNode node6;
    node6.coordinate = {9.0f, 2.0f};
    SolutionGraphNode node7;
    node7.coordinate = {9.6f, 1.2f};
    SolutionGraphNode node8;
    node8.coordinate = {10.5f, 0.8f};
    SolutionGraphNode node9;
    node9.coordinate = {11.5f, 0.8f};
    SolutionGraphNode node10;
    node10.coordinate = {12.3f, 1.2f};
    SolutionGraphNode node11;
    node11.coordinate = {12.6f, 1.8f};
    SolutionGraphNode node12;
    node12.coordinate = {12.9f, 2.5f};
    SolutionGraphNode node13;
    node13.coordinate = {12.9f, 3.6f};
    SolutionGraphNode node14;
    node14.coordinate = {12.9f, 5.8f};
    SolutionGraphNode node15;
    node15.coordinate = {12.9f, 9.1f};
    SolutionGraphNode node16;
    node16.coordinate = {12.9f, 11.4f};
    SolutionGraphNode node17;
    node17.coordinate = {12.4f, 12.2f};
    SolutionGraphNode node18;
    node18.coordinate = {11.8f, 12.6f};
    SolutionGraphNode node19;
    node19.coordinate = {11.2f, 12.8f};
    SolutionGraphNode node20;
    node20.coordinate = {10.4f, 12.8f};
    SolutionGraphNode node21;
    node21.coordinate = {9.8f, 12.5f};
    SolutionGraphNode node22;
    node22.coordinate = {9.2f, 11.8f};
    SolutionGraphNode node23;
    node23.coordinate = {9.0f, 11.3f};
    SolutionGraphNode node24;
    node24.coordinate = {9.2f, 10.0f};
    SolutionGraphNode node25;
    node25.coordinate = {9.7f, 9.2f};
    SolutionGraphNode node26;
    node26.coordinate = {10.1f, 8.3f};
    SolutionGraphNode node27;
    node27.coordinate = {10.1f, 7.4f};
    SolutionGraphNode node28;
    node28.coordinate = {9.6f, 6.6f};
    SolutionGraphNode node29;
    node29.coordinate = {8.0f, 5.1f};
    SolutionGraphNode node30;
    node30.coordinate = {7.0f, 4.1f};
    SolutionGraphNode node31;
    node31.coordinate = {5.4f, 2.7f};
    SolutionGraphNode node32;
    node32.coordinate = {4.4f, 1.8f};
    SolutionGraphNode node33;
    node33.coordinate = {3.3f, 1.6f};
    SolutionGraphNode node34;
    node34.coordinate = {2.7f, 1.9f};
    SolutionGraphNode node35;
    node35.coordinate = {2.1f, 2.3f};
    SolutionGraphNode node36;
    node36.coordinate = {1.8f, 3.0f};
    SolutionGraphNode node37;
    node37.coordinate = {1.6f, 4.0f};
    SolutionGraphNode node38;
    node38.coordinate = {1.8f, 5.0f};
    SolutionGraphNode node39;
    node39.coordinate = {3.6f, 6.9f};
    SolutionGraphNode node40;
    node40.coordinate = {5.2f, 8.4f};
    SolutionGraphNode node41;
    node41.coordinate = {6.0f, 9.4f};
    SolutionGraphNode node42;
    node42.coordinate = {6.2f, 10.1f};
    SolutionGraphNode node43;
    node43.coordinate = {6.1f, 11.0f};
    SolutionGraphNode node44;
    node44.coordinate = {5.7f, 11.8f};
    SolutionGraphNode node45;
    node45.coordinate = {4.9f, 12.3f};
    SolutionGraphNode node46;
    node46.coordinate = {4.0f, 12.5f};
    SolutionGraphNode node47;
    node47.coordinate = {3.0f, 12.4f};
    SolutionGraphNode node48;
    node48.coordinate = {2.3f, 12.0f};
    SolutionGraphNode node49;
    node49.coordinate = {1.9f, 11.4f};
    SolutionGraphNode node50;
    node50.coordinate = {1.6f, 10.5f};
    SolutionGraphNode node51;
    node51.coordinate = {1.8f, 9.8f};
    SolutionGraphNode node52;
    node52.coordinate = {2.3f, 9.1f};
    SolutionGraphNode node53;
    node53.coordinate = {2.8f, 8.5f};
    SolutionGraphNode node54;
    node54.coordinate = {3.5f, 7.8f};
    SolutionGraphNode node55;
    node55.coordinate = {4.0f, 7.1f};
    SolutionGraphNode node56;
    node56.coordinate = {4.8f, 6.5f};
    SolutionGraphNode node57;
    node57.coordinate = {5.4f, 5.8f};
    SolutionGraphNode node58;
    node58.coordinate = {6.8f, 4.3f};

    solutionGraph.push_back(node1);
    solutionGraph.push_back(node2);
    solutionGraph.push_back(node3);
    solutionGraph.push_back(node4);
    solutionGraph.push_back(node5);
    solutionGraph.push_back(node6);
    solutionGraph.push_back(node7);
    solutionGraph.push_back(node8);
    solutionGraph.push_back(node9);
    solutionGraph.push_back(node10);
    solutionGraph.push_back(node11);
    solutionGraph.push_back(node12);
    solutionGraph.push_back(node13);
    solutionGraph.push_back(node14);
    solutionGraph.push_back(node15);
    solutionGraph.push_back(node16);
    solutionGraph.push_back(node17);
    solutionGraph.push_back(node18);
    solutionGraph.push_back(node19);
    solutionGraph.push_back(node20);
    solutionGraph.push_back(node21);
    solutionGraph.push_back(node22);
    solutionGraph.push_back(node23);
    solutionGraph.push_back(node24);
    solutionGraph.push_back(node25);
    solutionGraph.push_back(node26);
    solutionGraph.push_back(node27);
    solutionGraph.push_back(node28);
    solutionGraph.push_back(node29);
    solutionGraph.push_back(node30);
    solutionGraph.push_back(node31);
    solutionGraph.push_back(node32);
    solutionGraph.push_back(node33);
    solutionGraph.push_back(node34);
    solutionGraph.push_back(node35);
    solutionGraph.push_back(node36);
    solutionGraph.push_back(node37);
    solutionGraph.push_back(node38);
    solutionGraph.push_back(node39);
    solutionGraph.push_back(node40);
    solutionGraph.push_back(node41);
    solutionGraph.push_back(node42);
    solutionGraph.push_back(node43);
    solutionGraph.push_back(node44);
    solutionGraph.push_back(node45);
    solutionGraph.push_back(node46);
    solutionGraph.push_back(node47);
    solutionGraph.push_back(node48);
    solutionGraph.push_back(node49);
    solutionGraph.push_back(node50);
    solutionGraph.push_back(node51);
    solutionGraph.push_back(node52);
    solutionGraph.push_back(node53);
    solutionGraph.push_back(node54);
    solutionGraph.push_back(node55);
    solutionGraph.push_back(node56);
    solutionGraph.push_back(node57);
    solutionGraph.push_back(node58);

    // Setting neighbors for node1
    node1.neighbors.push_back(&node58);
    node1.neighbors.push_back(&node2);
    // Setting neighbors for node2
    node2.neighbors.push_back(&node1);
    node2.neighbors.push_back(&node3);
    // Setting neighbors for node3
    node3.neighbors.push_back(&node2);
    node3.neighbors.push_back(&node4);
    // Setting neighbors for node4
    node4.neighbors.push_back(&node3);
    node4.neighbors.push_back(&node5);
    // Setting neighbors for node5
    node5.neighbors.push_back(&node4);
    node5.neighbors.push_back(&node6);
    // Setting neighbors for node6
    node6.neighbors.push_back(&node5);
    node6.neighbors.push_back(&node7);
    // Setting neighbors for node7
    node7.neighbors.push_back(&node6);
    node7.neighbors.push_back(&node8);
    // Setting neighbors for node8
    node8.neighbors.push_back(&node7);
    node8.neighbors.push_back(&node9);
    // Setting neighbors for node9
    node9.neighbors.push_back(&node8);
    node9.neighbors.push_back(&node10);
    // Setting neighbors for node10
    node10.neighbors.push_back(&node9);
    node10.neighbors.push_back(&node11);
    // Setting neighbors for node11
    node11.neighbors.push_back(&node10);
    node11.neighbors.push_back(&node12);
    // Setting neighbors for node12
    node12.neighbors.push_back(&node11);
    node12.neighbors.push_back(&node13);
    // Setting neighbors for node13
    node13.neighbors.push_back(&node12);
    node13.neighbors.push_back(&node14);
    // Setting neighbors for node14
    node14.neighbors.push_back(&node13);
    node14.neighbors.push_back(&node15);
    // Setting neighbors for node15
    node15.neighbors.push_back(&node14);
    node15.neighbors.push_back(&node16);
    // Setting neighbors for node16
    node16.neighbors.push_back(&node15);
    node16.neighbors.push_back(&node17);
    // Setting neighbors for node17
    node17.neighbors.push_back(&node16);
    node17.neighbors.push_back(&node18);
    // Setting neighbors for node18
    node18.neighbors.push_back(&node17);
    node18.neighbors.push_back(&node19);
    // Setting neighbors for node19
    node19.neighbors.push_back(&node18);
    node19.neighbors.push_back(&node20);
    // Setting neighbors for node20
    node20.neighbors.push_back(&node19);
    node20.neighbors.push_back(&node21);
    // Setting neighbors for node21
    node21.neighbors.push_back(&node20);
    node21.neighbors.push_back(&node22);
    // Setting neighbors for node22
    node22.neighbors.push_back(&node21);
    node22.neighbors.push_back(&node23);
    // Setting neighbors for node23
    node23.neighbors.push_back(&node22);
    node23.neighbors.push_back(&node24);
    // Setting neighbors for node24
    node24.neighbors.push_back(&node23);
    node24.neighbors.push_back(&node25);
    // Setting neighbors for node25
    node25.neighbors.push_back(&node24);
    node25.neighbors.push_back(&node26);
    // Setting neighbors for node26
    node26.neighbors.push_back(&node25);
    node26.neighbors.push_back(&node27);
    // Setting neighbors for node27
    node27.neighbors.push_back(&node26);
    node27.neighbors.push_back(&node28);
    // Setting neighbors for node28
    node28.neighbors.push_back(&node27);
    node28.neighbors.push_back(&node29);
    // Setting neighbors for node29
    node29.neighbors.push_back(&node28);
    node29.neighbors.push_back(&node30);
    // Setting neighbors for node30
    node30.neighbors.push_back(&node29);
    node30.neighbors.push_back(&node31);
    // Setting neighbors for node31
    node31.neighbors.push_back(&node30);
    node31.neighbors.push_back(&node32);
    // Setting neighbors for node32
    node32.neighbors.push_back(&node31);
    node32.neighbors.push_back(&node33);
    // Setting neighbors for node33
    node33.neighbors.push_back(&node32);
    node33.neighbors.push_back(&node34);
    // Setting neighbors for node34
    node34.neighbors.push_back(&node33);
    node34.neighbors.push_back(&node35);
    // Setting neighbors for node35
    node35.neighbors.push_back(&node34);
    node35.neighbors.push_back(&node36);
    // Setting neighbors for node36
    node36.neighbors.push_back(&node35);
    node36.neighbors.push_back(&node37);
    // Setting neighbors for node37
    node37.neighbors.push_back(&node36);
    node37.neighbors.push_back(&node38);
    // Setting neighbors for node38
    node38.neighbors.push_back(&node37);
    node38.neighbors.push_back(&node39);
    // Setting neighbors for node39
    node39.neighbors.push_back(&node38);
    node39.neighbors.push_back(&node40);
    // Setting neighbors for node40
    node40.neighbors.push_back(&node39);
    node40.neighbors.push_back(&node41);
    // Setting neighbors for node41
    node41.neighbors.push_back(&node40);
    node41.neighbors.push_back(&node42);
    // Setting neighbors for node42
    node42.neighbors.push_back(&node41);
    node42.neighbors.push_back(&node43);
    // Setting neighbors for node43
    node43.neighbors.push_back(&node42);
    node43.neighbors.push_back(&node44);
    // Setting neighbors for node44
    node44.neighbors.push_back(&node43);
    node44.neighbors.push_back(&node45);
    // Setting neighbors for node45
    node45.neighbors.push_back(&node44);
    node45.neighbors.push_back(&node46);
    // Setting neighbors for node46
    node46.neighbors.push_back(&node45);
    node46.neighbors.push_back(&node47);
    // Setting neighbors for node47
    node47.neighbors.push_back(&node46);
    node47.neighbors.push_back(&node48);
    // Setting neighbors for node48
    node48.neighbors.push_back(&node47);
    node48.neighbors.push_back(&node49);
    // Setting neighbors for node49
    node49.neighbors.push_back(&node48);
    node49.neighbors.push_back(&node50);
    // Setting neighbors for node50
    node50.neighbors.push_back(&node49);
    node50.neighbors.push_back(&node51);
    // Setting neighbors for node51
    node51.neighbors.push_back(&node50);
    node51.neighbors.push_back(&node52);
    // Setting neighbors for node52
    node52.neighbors.push_back(&node51);
    node52.neighbors.push_back(&node53);
    // Setting neighbors for node53
    node53.neighbors.push_back(&node52);
    node53.neighbors.push_back(&node54);
    // Setting neighbors for node54
    node54.neighbors.push_back(&node53);
    node54.neighbors.push_back(&node55);
    // Setting neighbors for node55
    node55.neighbors.push_back(&node54);
    node55.neighbors.push_back(&node56);
    // Setting neighbors for node56
    node56.neighbors.push_back(&node55);
    node56.neighbors.push_back(&node57);
    // Setting neighbors for node57
    node57.neighbors.push_back(&node56);
    node57.neighbors.push_back(&node58);
    // Setting neighbors for node58
    node58.neighbors.push_back(&node57);
    node58.neighbors.push_back(&node1);

}
