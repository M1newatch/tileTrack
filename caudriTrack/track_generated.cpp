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
        {4.9f, 7.23f},  // start
        {5.88f, 8.14f},
        {6.41f, 8.79f},
        {6.71f, 9.42f},
        {6.88f, 10.2f},
        {6.84f, 10.84f},
        {6.75f, 11.18f},
        {6.63f, 11.53f},
        {6.4f, 11.86f},
        {6.01f, 12.35f},
        {5.35f, 12.76f},
        {4.55f, 13.05f},
        {3.84f, 13.07f},
        {3.12f, 13.02f},
        {2.47f, 12.77f},
        {1.99f, 12.48f},
        {1.6f, 12.07f},
        {1.37f, 11.71f},
        {1.19f, 11.25f},
        {1.09f, 10.69f},
        {1.12f, 10.12f},
        {1.33f, 9.35f},
        {1.6f, 8.95f},
        {2.04f, 8.48f},
        {2.5f, 8.04f},
        {3.19f, 7.28f},
        {2.43f, 6.58f},
        {1.69f, 5.83f},
        {1.3f, 5.25f},
        {1.1f, 4.71f},
        {1.02f, 4.09f},
        {1.02f, 3.45f},
        {1.15f, 2.86f},
        {1.37f, 2.32f},
        {1.73f, 1.83f},
        {2.17f, 1.43f},
        {2.94f, 1.06f},
        {3.58f, 0.97f},
        {4.19f, 1.04f},
        {4.68f, 1.18f},
        {5.09f, 1.41f},
        {5.48f, 1.76f},
        {7.02f, 3.2f},
        {8.99f, 1.13f},
        {9.6f, 0.51f},
        {10.07f, 0.25f},
        {10.47f, 0.15f},
        {10.89f, 0.1f},
        {11.29f, 0.1f},
        {11.81f, 0.2f},
        {12.22f, 0.36f},
        {12.55f, 0.58f},
        {12.83f, 0.84f},
        {13.09f, 1.15f},
        {13.24f, 1.38f},
        {13.42f, 1.69f},
        {13.55f, 2.12f},
        {13.6f, 2.73f},
        {13.58f, 3.45f},
        {13.55f, 4.5f},
        {13.57f, 9.93f},
        {13.53f, 11.01f},
        {13.47f, 11.5f},
        {13.32f, 11.86f},
        {13.16f, 12.2f},
        {12.94f, 12.45f},
        {12.63f, 12.76f},
        {12.26f, 13.07f},
        {11.75f, 13.24f},
        {11.3f, 13.38f},
        {10.76f, 13.4f},
        {10.16f, 13.27f},
        {9.65f, 12.99f},
        {9.09f, 12.55f},
        {8.7f, 11.89f},
        {8.48f, 11.27f},
        {8.45f, 10.81f},
        {8.5f, 10.35f},
        {8.63f, 9.89f},
        {8.96f, 9.35f},
        {9.27f, 8.92f},
        {9.58f, 8.34f},
        {9.6f, 7.7f},
        {9.39f, 7.11f},
        {9.02f, 6.72f},
        {8.39f, 6.16f},
        {7.79f, 5.59f},
        {7.04f, 4.95f}   // end
    };

    Mazepolygon poly1;
    for (const auto& coord : coords1) {
        poly1.coordinates.push_back(coord);
    }

    polygons.push_back(poly1);

    // Polygon 2
    std::vector<MazeCoordinate> coords2 = {
        {4.03f, 8.15f},  // start
        {5.11f, 9.16f},
        {5.5f, 9.76f},
        {5.58f, 10.32f},
        {5.43f, 10.92f},
        {5.09f, 11.44f},
        {4.44f, 11.8f},
        {3.82f, 11.89f},
        {3.11f, 11.82f},
        {2.59f, 11.46f},
        {2.27f, 10.81f},
        {2.27f, 10.23f},
        {2.62f, 9.67f}   // end
    };

    Mazepolygon poly2;
    for (const auto& coord : coords2) {
        poly2.coordinates.push_back(coord);
    }

    polygons.push_back(poly2);

    // Polygon 3
    std::vector<MazeCoordinate> coords3 = {
        {3.95f, 6.43f},  // start
        {2.66f, 5.25f},
        {2.25f, 4.49f},
        {2.23f, 3.87f},
        {2.27f, 3.44f},
        {2.38f, 2.97f},
        {2.66f, 2.6f},
        {3.17f, 2.32f},
        {3.67f, 2.26f},
        {4.25f, 2.39f},
        {4.55f, 2.73f},
        {4.89f, 2.99f},
        {5.24f, 3.33f},
        {5.62f, 3.68f},
        {6.1f, 4.13f},
        {5.07f, 5.27f}   // end
    };

    Mazepolygon poly3;
    for (const auto& coord : coords3) {
        poly3.coordinates.push_back(coord);
    }

    polygons.push_back(poly3);

    // Polygon 4
    std::vector<MazeCoordinate> coords4 = {
        {7.86f, 4.08f},  // start
        {10.05f, 1.74f},
        {10.7f, 1.38f},
        {11.3f, 1.4f},
        {11.75f, 1.59f},
        {12.03f, 1.91f},
        {12.29f, 2.43f},
        {12.33f, 2.99f},
        {12.31f, 3.89f},
        {12.31f, 10.75f},
        {12.14f, 11.55f},
        {11.62f, 12.08f},
        {11.06f, 12.23f},
        {10.37f, 12.1f},
        {9.84f, 11.65f},
        {9.66f, 10.99f},
        {9.84f, 10.23f},
        {10.31f, 9.55f},
        {10.63f, 8.92f},
        {10.78f, 8.17f},
        {10.7f, 7.48f},
        {10.48f, 6.73f},
        {10.16f, 6.23f}   // end
    };

    Mazepolygon poly4;
    for (const auto& coord : coords4) {
        poly4.coordinates.push_back(coord);
    }

    polygons.push_back(poly4);

    // Solution path 1 nodes
    SolutionGraphNode node1;
    node1.coordinate = {7.38f, 3.7f};
    SolutionGraphNode node2;
    node2.coordinate = {7.46f, 3.59f};
    SolutionGraphNode node3;
    node3.coordinate = {7.85f, 3.2f};
    SolutionGraphNode node4;
    node4.coordinate = {8.22f, 2.76f};
    SolutionGraphNode node5;
    node5.coordinate = {8.64f, 2.34f};
    SolutionGraphNode node6;
    node6.coordinate = {8.97f, 1.95f};
    SolutionGraphNode node7;
    node7.coordinate = {9.64f, 1.25f};
    SolutionGraphNode node8;
    node8.coordinate = {10.53f, 0.76f};
    SolutionGraphNode node9;
    node9.coordinate = {11.51f, 0.76f};
    SolutionGraphNode node10;
    node10.coordinate = {12.27f, 1.17f};
    SolutionGraphNode node11;
    node11.coordinate = {12.65f, 1.75f};
    SolutionGraphNode node12;
    node12.coordinate = {12.88f, 2.49f};
    SolutionGraphNode node13;
    node13.coordinate = {12.92f, 3.63f};
    SolutionGraphNode node14;
    node14.coordinate = {12.92f, 5.82f};
    SolutionGraphNode node15;
    node15.coordinate = {12.92f, 9.14f};
    SolutionGraphNode node16;
    node16.coordinate = {12.85f, 11.41f};
    SolutionGraphNode node17;
    node17.coordinate = {12.39f, 12.2f};
    SolutionGraphNode node18;
    node18.coordinate = {11.76f, 12.65f};
    SolutionGraphNode node19;
    node19.coordinate = {11.16f, 12.81f};
    SolutionGraphNode node20;
    node20.coordinate = {10.43f, 12.79f};
    SolutionGraphNode node21;
    node21.coordinate = {9.76f, 12.46f};
    SolutionGraphNode node22;
    node22.coordinate = {9.22f, 11.81f};
    SolutionGraphNode node23;
    node23.coordinate = {9.03f, 11.26f};
    SolutionGraphNode node24;
    node24.coordinate = {9.22f, 10f};
    SolutionGraphNode node25;
    node25.coordinate = {9.73f, 9.22f};
    SolutionGraphNode node26;
    node26.coordinate = {10.15f, 8.34f};
    SolutionGraphNode node27;
    node27.coordinate = {10.11f, 7.45f};
    SolutionGraphNode node28;
    node28.coordinate = {9.61f, 6.6f};
    SolutionGraphNode node29;
    node29.coordinate = {8.02f, 5.1f};
    SolutionGraphNode node30;
    node30.coordinate = {7.03f, 4.06f};
    SolutionGraphNode node31;
    node31.coordinate = {5.45f, 2.71f};
    SolutionGraphNode node32;
    node32.coordinate = {4.37f, 1.8f};
    SolutionGraphNode node33;
    node33.coordinate = {3.29f, 1.64f};
    SolutionGraphNode node34;
    node34.coordinate = {2.67f, 1.85f};
    SolutionGraphNode node35;
    node35.coordinate = {2.07f, 2.32f};
    SolutionGraphNode node36;
    node36.coordinate = {1.75f, 3.05f};
    SolutionGraphNode node37;
    node37.coordinate = {1.63f, 3.95f};
    SolutionGraphNode node38;
    node38.coordinate = {1.84f, 5.03f};
    SolutionGraphNode node39;
    node39.coordinate = {3.59f, 6.94f};
    SolutionGraphNode node40;
    node40.coordinate = {5.18f, 8.42f};
    SolutionGraphNode node41;
    node41.coordinate = {5.98f, 9.37f};
    SolutionGraphNode node42;
    node42.coordinate = {6.17f, 10.14f};
    SolutionGraphNode node43;
    node43.coordinate = {6.08f, 10.96f};
    SolutionGraphNode node44;
    node44.coordinate = {5.69f, 11.75f};
    SolutionGraphNode node45;
    node45.coordinate = {4.87f, 12.27f};
    SolutionGraphNode node46;
    node46.coordinate = {3.98f, 12.49f};
    SolutionGraphNode node47;
    node47.coordinate = {2.97f, 12.43f};
    SolutionGraphNode node48;
    node48.coordinate = {2.32f, 12.03f};
    SolutionGraphNode node49;
    node49.coordinate = {1.86f, 11.35f};
    SolutionGraphNode node50;
    node50.coordinate = {1.63f, 10.54f};
    SolutionGraphNode node51;
    node51.coordinate = {1.83f, 9.77f};
    SolutionGraphNode node52;
    node52.coordinate = {2.33f, 9.11f};
    SolutionGraphNode node53;
    node53.coordinate = {2.79f, 8.52f};
    SolutionGraphNode node54;
    node54.coordinate = {3.52f, 7.83f};
    SolutionGraphNode node55;
    node55.coordinate = {3.95f, 7.1f};
    SolutionGraphNode node56;
    node56.coordinate = {4.82f, 6.48f};
    SolutionGraphNode node57;
    node57.coordinate = {5.38f, 5.76f};
    SolutionGraphNode node58;
    node58.coordinate = {6.8f, 4.28f};

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
