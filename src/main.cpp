#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <random>
#include <string>
#include <cmath>
#include <iomanip>
#include <set>
#include <tuple>
#include <map>
#include <array>
#include <queue>
#include <algorithm>
#include "maze_structs.h"
#include "json.hpp"
#include <limits>
#include <stack>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using json = nlohmann::json;
unsigned int GRID_SIZE = 0;


/**
 * @brief representation of a tile 
 * @var edges: open (O)/closed (X) connections on the tile edges 
 * 
 */
struct Tile {
    std::string name;
    std::vector<std::vector<std::pair<float, float>>> walls;
    std::vector<std::array<std::string, 4>> edges;
    int index = -1;
    std::vector<std::vector<int>> validNeighbors;
};

/**
 * @brief Checks if two edge match with closed or open connections
 * @return true if edges match (open-to-open or wall-to-wall), false otherwise
 */
bool edgesMatch(const std::string& a, const std::string& b) {
    // Only allow open-to-open and wall-to-wall
    return (a == "O" && b == "O") || (a == "X" && b == "X");
}

/**
 * @brief Rotates the walls of a tile so the
 * wall positions are updated according to the rotation.
 *
 * @param walls The original wall positions.
 * @param rot The rotation angle in 90-degree increments (0-3).
 */
std::vector<std::vector<std::pair<float, float>>> rotateWalls(
    const std::vector<std::vector<std::pair<float, float>>>& walls, int rot) {
    std::vector<std::vector<std::pair<float, float>>> result = walls;
    float angle = rot * (M_PI / 2.0f);
    float cos_a = std::cos(angle);
    float sin_a = std::sin(angle);
    for (auto& wall : result) {
        for (auto& p : wall) {
            float x = p.first - 0.5f;
            float y = p.second - 0.5f;
            float x_new = cos_a * x - sin_a * y;
            float y_new = sin_a * x + cos_a * y;
            p.first = x_new + 0.5f;
            p.second = y_new + 0.5f;
        }
    }
    return result;
}

// load tileset
std::vector<Tile> loadTilesetWithEdges(const std::string& filename) {
    std::ifstream file(filename);
    json j;
    file >> j;
    std::vector<Tile> baseTiles;
    std::vector<Tile> allTiles;
    int idx = 0;
    for (const auto& t : j) {
        Tile tile;
        tile.name = t["name"];
        for (const auto& wall : t["walls"]) {
            std::vector<std::pair<float, float>> wall_coords;
            const float padding = 0.0025f; // 5% padding on each side
            for (const auto& coord : wall) {
                // Move each coordinate towards the center (0.5, 0.5)
                float x = coord[0];
                float y = coord[1];
                float x_padded = 0.5f + (x - 0.5f) * (1.0f - 2.0f * padding);
                float y_padded = 0.5f + (y - 0.5f) * (1.0f - 2.0f * padding);
                wall_coords.emplace_back(x_padded, y_padded);
            }
            tile.walls.push_back(wall_coords);
        }
        if (t.contains("edges")) {
            for (const auto& e : t["edges"]) {
                std::array<std::string, 4> arr;
                for (int i = 0; i < 4; ++i) arr[i] = e[i];
                tile.edges.push_back(arr);
            }
        } else {
            std::cout << "Missing edge definitions for tile: " << tile.name << std::endl;
            tile.edges.push_back({"X", "X", "X", "X"});
        }
        tile.index = idx++;
        baseTiles.push_back(tile);
    }
    // generate all rotations and filter duplicates
    for (const auto& tile : baseTiles) {
        for (int i = 0; i < tile.edges.size(); ++i) {
            Tile t = tile;
            t.edges.clear();
            t.edges.push_back(tile.edges[i]);
            int extra_rot = 0;
            if (tile.name.find("curve") != std::string::npos) extra_rot = 1;
            if (tile.name.find("endcap") != std::string::npos) extra_rot = 2;
            t.walls = rotateWalls(tile.walls, (i + extra_rot) % 4);
            t.name = tile.name + "_rot" + std::to_string(i*90);
            t.index = allTiles.size();
            allTiles.push_back(t);
        }
    }
    // calculate adjacency list for each tile
    for (int t = 0; t < allTiles.size(); ++t) {
        allTiles[t].validNeighbors.resize(4);
        for (int dir = 0; dir < 4; ++dir) {
            for (int n = 0; n < allTiles.size(); ++n) {
                if (edgesMatch(allTiles[t].edges[0][dir], allTiles[n].edges[0][(dir+2)%4])) {
                    allTiles[t].validNeighbors[dir].push_back(n);
                }
            }
        }
    }
    return allTiles;
}

struct Cell {
    bool collapsed = false;
    std::vector<int> options;
};

// bounds for tiles so they dont overlap
struct Bounds {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    void update(float x, float y) {
        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x);
        maxY = std::max(maxY, y);
    }
    float width() const { return maxX - minX; }
    float height() const { return maxY - minY; }
};

Bounds calculateBounds(const std::list<Mazepolygon>& polygons) {
    Bounds bounds;

    float padding = 0.01f;
    bounds.minX += padding;
    bounds.minY += padding;
    bounds.maxX -= padding;
    bounds.maxY -= padding;

    for (const auto& poly : polygons) {
        for (const auto& coord : poly.coordinates) {
            bounds.update(coord.x, coord.y);
        }
    }
    return bounds;
}

// Hilfsstruktur für Kanten
using Edge = std::pair<std::pair<int,int>, std::pair<int,int>>;

void exportSVG(const std::list<Mazepolygon>& polygons, const std::string& filename, const std::vector<Edge>& solutionEdges, int DIM) {
    std::ofstream svg(filename);
    Bounds bounds = calculateBounds(polygons);
    float scale = std::min(1000.0f / bounds.width(), 1000.0f / bounds.height());
    char header[512];
    snprintf(header, sizeof(header), "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\" viewBox=\"%f %f %f %f\">\n    <rect width=\"100%%\" height=\"100%%\" fill=\"#f8f8f8\"/>\n", 1000, 1000, bounds.minX, bounds.minY, bounds.width(), bounds.height());
    svg << header;
    // Grid zeichnen
    svg << "    <g stroke=\"#bbb\" stroke-width=\"0.02\">\n";
    for (int i = 0; i <= GRID_SIZE; ++i) {
        float x = i * 1.0f;
        svg << "        <line x1=\"" << x << "\" y1=\"0\" x2=\"" << x << "\" y2=\"" << GRID_SIZE << "\" />\n";
        svg << "        <line x1=\"0\" y1=\"" << x << "\" x2=\"" << GRID_SIZE << "\" y2=\"" << x << "\" />\n";
    }
    svg << "    </g>\n";
    svg << "    <circle cx=\"0\" cy=\"0\" r=\"0.2\" fill=\"red\"/>\n";
    // Lösungslinien
    svg << "    <g stroke=\"#d22\" stroke-width=\"0.05\">\n";
    for (const auto& edge : solutionEdges) {
        float x1 = edge.first.first + 0.5f;
        float y1 = edge.first.second + 0.5f;
        float x2 = edge.second.first + 0.5f;
        float y2 = edge.second.second + 0.5f;
        svg << "        <line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" />\n";
    }
    svg << "    </g>\n";
    for (const auto& poly : polygons) {
        if (poly.coordinates.empty()) continue;
        svg << "    <path d=\"M ";
        bool first = true;
        for (const auto& coord : poly.coordinates) {
            if (!first) svg << " L ";
            svg << coord.x << "," << coord.y;
            first = false;
        }
        svg << " Z\" fill=\"#333333\" stroke=\"none\"/>\n";
    }
    svg << "</svg>";
    svg.close();
}

// Transformiere Koordinaten (Hilfsfunktion)
std::vector<std::pair<float, float>> transform(const std::vector<std::pair<float, float>>& coords, float dx, float dy, float angle_rad) {
    std::vector<std::pair<float, float>> result;
    float cos_a = std::cos(angle_rad);
    float sin_a = std::sin(angle_rad);
    for (const auto& [x, y] : coords) {
        float x_new = cos_a * x - sin_a * y + dx;
        float y_new = sin_a * x + cos_a * y + dy;
        result.emplace_back(x_new, y_new);
    }
    return result;
}

// Maze-Generierung mit DFS
struct MazeCell {
    bool visited = false;
    bool walls[4] = {true, true, true, true}; // 0: rechts, 1: unten, 2: links, 3: oben
};

void generateMaze(std::vector<std::vector<MazeCell>>& maze, int DIM, std::mt19937& rng, std::vector<Edge>& solutionEdges) {
    std::stack<std::pair<int, int>> stack;
    stack.push({0, 0});
    maze[0][0].visited = true;
    std::uniform_int_distribution<int> dirDist(0, 3);
    const int dx[4] = {1, 0, -1, 0};
    const int dy[4] = {0, 1, 0, -1};
    while (!stack.empty()) {
        int x = stack.top().first, y = stack.top().second;
        std::vector<int> dirs = {0, 1, 2, 3};
        std::shuffle(dirs.begin(), dirs.end(), rng);
        bool moved = false;
        for (int dir : dirs) {
            int nx = x + dx[dir], ny = y + dy[dir];
            if (nx >= 0 && nx < DIM && ny >= 0 && ny < DIM && !maze[ny][nx].visited) {
                maze[y][x].walls[dir] = false;
                maze[ny][nx].walls[(dir+2)%4] = false;
                maze[ny][nx].visited = true;
                stack.push({nx, ny});
                // Kante zum Lösungsgraphen hinzufügen
                solutionEdges.push_back({{x, y}, {nx, ny}});
                moved = true;
                break;
            }
        }
        if (!moved) stack.pop();
    }
}

void addCrossings(std::vector<std::vector<MazeCell>>& maze, int DIM, float crossingProbability, std::mt19937& rng, std::vector<Edge>& solutionEdges) {
    std::uniform_real_distribution<float> probDist(0.0f, 1.0f);
    for (int y = 1; y < DIM-1; ++y) {
        for (int x = 1; x < DIM-1; ++x) {
            int openCount = 0;
            for (int d = 0; d < 4; ++d) {
                if (!maze[y][x].walls[d]) openCount++;
            }
            if (openCount < 4 && probDist(rng) < crossingProbability) {
                // Open all walls to make a fourway crossing
                for (int d = 0; d < 4; ++d) {
                    if (maze[y][x].walls[d]) { // Only add if wall was closed
                        maze[y][x].walls[d] = false;
                        int nx = x + (d == 0 ? 1 : d == 2 ? -1 : 0);
                        int ny = y + (d == 1 ? 1 : d == 3 ? -1 : 0);
                        if (nx >= 0 && nx < DIM && ny >= 0 && ny < DIM) {
                            maze[ny][nx].walls[(d+2)%4] = false;
                            // Add to solutionEdges if not already present
                            Edge e1 = {{x, y}, {nx, ny}};
                            Edge e2 = {{nx, ny}, {x, y}};
                            if (std::find(solutionEdges.begin(), solutionEdges.end(), e1) == solutionEdges.end() &&
                                std::find(solutionEdges.begin(), solutionEdges.end(), e2) == solutionEdges.end()) {
                                solutionEdges.push_back(e1);
                            }
                        }
                    }
                }
            }
        }
    }
}

// Finde das passende Tile und Rotation für eine Maze-Zelle
int findMatchingTile(const std::vector<Tile>& tiles, const bool walls[4], int x = -1, int y = -1) {
    for (int t = 0; t < tiles.size(); ++t) {
        for (int rot = 0; rot < 4; ++rot) {
            const auto& edge = tiles[t].edges[0];
            bool match = true;
            for (int d = 0; d < 4; ++d) {
                // Wand: edge muss "X", offen: edge muss "O"
                if (walls[d]) {
                    if (edge[d] != "X") { match = false; break; }
                } else {
                    if (edge[d] != "O") { match = false; break; }
                }
            }
            if (match) return t;
        }
    }
    // Debug-Ausgabe für fehlende Kombinationen
    if (x >= 0 && y >= 0) {
        std::cout << "Kein passendes Tile für Zelle (" << x << "," << y << ") mit Wänden: ";
        for (int d = 0; d < 4; ++d) std::cout << (walls[d] ? "1" : "0");
        std::cout << std::endl;
    }
    return -1; // Kein passendes Tile gefunden
}

int main(int argc, char* argv[]) {
    if (argc < 3 ) {
        std::cout << "Usage: " << argv[0] << " <tileset.json> <output.cpp> [grid_size] [crossing probability] [seed]\n";
        return 1;
    }
    std::string tileset_file = argv[1];
    std::string output_file = argv[2];
    unsigned int GRID_SIZE = argc > 3 ? std::stoul(argv[3]) : 10;
    float crossing_probability = argc > 4 ? std::stof(argv[4]) : 0.1f;
    float seed = argc > 5 ? std::stof(argv[5]) : std::random_device{}();
    std::mt19937 rng(seed);

    auto tiles = loadTilesetWithEdges(tileset_file);
    int DIM = GRID_SIZE;
    // Maze generieren
    std::vector<std::vector<MazeCell>> maze(DIM, std::vector<MazeCell>(DIM));
    std::vector<Edge> solutionEdges;
    generateMaze(maze, DIM, rng, solutionEdges);
    addCrossings(maze, DIM, crossing_probability, rng, solutionEdges);

    // Grid für Tiles
    std::vector<std::vector<Cell>> grid(DIM, std::vector<Cell>(DIM));
    for (int y = 0; y < DIM; ++y) {
        for (int x = 0; x < DIM; ++x) {
            int tileIdx = findMatchingTile(tiles, maze[y][x].walls, x, y);
            if (tileIdx >= 0) {
                grid[y][x].collapsed = true;
                grid[y][x].options = {tileIdx};
            } else {
                // Fallback: Blocktile
                int block_index = -1;
                for (int i = 0; i < tiles.size(); ++i) {
                    if (tiles[i].name.find("block") != std::string::npos) {
                        block_index = i;
                        break;
                    }
                }
                grid[y][x].collapsed = true;
                grid[y][x].options = {block_index};
            }
        }
    }

    // Polygone generieren
    std::list<Mazepolygon> polygons;
    for (int y = 0; y < DIM; ++y) {
        for (int x = 0; x < DIM; ++x) {
            const auto& cell = grid[y][x];
            if (!cell.collapsed) continue;
            const Tile& tile = tiles[cell.options[0]];
            float gx = x * 1.0f;
            float gy = y * 1.0f;
            float angle = 0.0f; // Walls sind schon rotiert
            for (const auto& wall : tile.walls) {

                auto global_coords = transform(wall, gx, gy, angle);
                Mazepolygon poly;
                for (const auto& [gx, gy] : global_coords) {
                    poly.coordinates.push_back(MazeCoordinate{gx, gy});
                }
                polygons.push_back(poly);
            }
        }
    }

    exportSVG(polygons, "track.svg", solutionEdges, DIM);
    std::ofstream out(output_file);
    out << "#include <iostream>\n#include <list>\n#include <cmath>\n#include <vector>\n#include <unordered_map>\n\n#include \"maze_generator.h\"\n\n";
    out << "void MazeGenerator::generateMaze() {\n";
    int idx = 0;
    for (const auto& poly : polygons) {
        out << "    Mazepolygon poly" << idx << ";\n";
        out << "    poly" << idx << ".coordinates = {";
        for (const auto& coord : poly.coordinates) {
            out << "{" << std::fixed << std::setprecision(5) << coord.x << "f, " 
                << coord.y << "f}, ";
        }
        out << "};\n";
        out << "    polygons.push_back(poly" << idx << ");\n";
        idx++;
    }

    // Build a mapping from node coordinates to their indices and count occurrences
    std::map<std::pair<int, int>, int> coord_to_index;
    std::map<std::pair<int, int>, int> coord_count;
    int node_idx = 1;
    for (const auto& edge : solutionEdges) {
        coord_to_index[edge.first] = node_idx;
        coord_count[edge.first]++;
        node_idx++;
    }
    // Write SolutionGraphNodes
    int j = 0;
    for (const auto& edge : solutionEdges) {
        j++;
        out << "    SolutionGraphNode node" << j << ";\n";
    }
    // Write coordinates and neighbors
    int i = 0;
    for (const auto& edge : solutionEdges) {
        i++;
        out << "    node" << i << ".coordinate = {" << edge.first.first + 0.5f << "f, " << edge.first.second + 0.5f << "f};\n";
        // Determine neighbors
        auto curr = edge.first;
        int prev_idx = (i > 1) ? i - 1 : -1;
        int next_idx = (i < solutionEdges.size()) ? i + 1 : -1;
        if (coord_count[curr] == 1) {
            // Dead end: only previous neighbor
            if (prev_idx > 0)
                out << "    node" << i << ".neighbors.push_back(&node" << prev_idx << ");\n";
        } else {
            // Normal: previous and next neighbor
            if (prev_idx > 0)
                out << "    node" << i << ".neighbors.push_back(&node" << prev_idx << ");\n";
            if (next_idx > 0 && next_idx <= solutionEdges.size())
                out << "    node" << i << ".neighbors.push_back(&node" << next_idx << ");\n";
        }
        out << "    solutionGraph.push_back(node" << i << ");\n";
    }
    out << "}\n";
    out.close();
    std::cout << "SVG-Visualisierung in track3.svg gespeichert.\n";
    std::cout << "Track generiert und in " << output_file << " geschrieben.\n";
    std::cout << "Anzahl der generierten Polygone: " << polygons.size() << "\n";
    return 0;
}