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
int GRID_SIZE = 0;

struct Tile {
    std::string name;
    std::vector<std::vector<std::pair<float, float>>> walls;
    std::vector<std::array<std::string, 4>> edges; // edges for each connection/rotation
    int index = -1; // for reference
    std::vector<std::vector<int>> validNeighbors; // [4][tileIndex]
};

// Prüfe, ob zwei Kanten kompatibel sind
bool edgesMatch(const std::string& a, const std::string& b) {
    // Only allow open-to-open and wall-to-wall
    return (a == "O" && b == "O") || (a == "X" && b == "X");
}

// Rotiert die Walls eines Tiles um das Zentrum (0.5, 0.5)
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

// Lade Tileset und erzeuge alle Rotationen
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
            for (const auto& coord : wall) {
                wall_coords.emplace_back(coord[0], coord[1]);
            }
            tile.walls.push_back(wall_coords);
        }
        // Edges: Wir nehmen die entry/exit_angle als Kanten-String (z.B. "0-180-0-180")
        // Alternativ: Manuell im JSON als "edges" angeben, hier generisch:
        if (t.contains("edges")) {
            // Optional: explizite Edges im JSON
            for (const auto& e : t["edges"]) {
                std::array<std::string, 4> arr;
                for (int i = 0; i < 4; ++i) arr[i] = e[i];
                tile.edges.push_back(arr);
            }
        } else if (t.contains("connections")) {
            // Generiere aus connections eine Kantenbeschreibung
            for (const auto& c : t["connections"]) {
                std::array<std::string, 4> arr = {"X","X","X","X"};
                int entry_dir = (static_cast<int>(std::round(c["entry_angle"].get<float>() / 90.0f)) % 4);
                int exit_dir  = (static_cast<int>(std::round(c["exit_angle"].get<float>() / 90.0f)) % 4);
                arr[entry_dir] = "O"; // Eingang
                arr[exit_dir]  = "O"; // Ausgang
                tile.edges.push_back(arr);
            }
        }
        tile.index = idx++;
        baseTiles.push_back(tile);
    }
    //Erzeuge alle Rotationen und filtere Duplikate
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
    // Nachbarschaftslisten berechnen
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
    std::vector<int> options; // tile indices
};

// Neue Propagation wie im Beispiel
void propagate(std::vector<std::vector<Cell>>& grid, const std::vector<Tile>& tiles, int DIM) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int y = 0; y < DIM; ++y) {
            for (int x = 0; x < DIM; ++x) {
                if (grid[y][x].collapsed) continue;
                std::vector<int> options = grid[y][x].options;
                // Für jede Richtung
                for (int dir = 0; dir < 4; ++dir) {
                    int nx = x, ny = y;
                    if (dir == 0) nx++; // rechts
                    if (dir == 1) ny++; // unten
                    if (dir == 2) nx--; // links
                    if (dir == 3) ny--; // oben
                    if (nx < 0 || nx >= DIM || ny < 0 || ny >= DIM) continue;
                    const auto& neighbor = grid[ny][nx];
                    std::set<int> valid;
                    for (int nOpt : neighbor.options) {
                        for (int v : tiles[nOpt].validNeighbors[(dir+2)%4]) {
                            valid.insert(v);
                        }
                    }
                    // Filtere options auf Schnittmenge mit valid
                    std::vector<int> newOptions;
                    for (int opt : options) {
                        if (valid.count(opt)) newOptions.push_back(opt);
                    }
                    if (newOptions.size() < options.size()) {
                        options = newOptions;
                        changed = true;
                    }
                }
                grid[y][x].options = options;
            }
        }
    }
}

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
    for (const auto& poly : polygons) {
        for (const auto& coord : poly.coordinates) {
            bounds.update(coord.x, coord.y);
        }
    }
    float padding = 0.1f;
    bounds.minX -= padding;
    bounds.minY -= padding;
    bounds.maxX += padding;
    bounds.maxY += padding;
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
        std::cout << "Usage: " << argv[0] << " <tileset.json> <output.cpp> [grid_size] [seed]\n";
        return 1;
    }
    std::string tileset_file = argv[1];
    std::string output_file = argv[2];
    unsigned int GRID_SIZE = argc > 3 ? std::stoul(argv[3]) : 10;
    unsigned int seed = argc > 4 ? std::stoul(argv[4]) : std::random_device{}();
    std::mt19937 rng(seed);

    auto tiles = loadTilesetWithEdges(tileset_file);
    int DIM = GRID_SIZE;
    // Maze generieren
    std::vector<std::vector<MazeCell>> maze(DIM, std::vector<MazeCell>(DIM));
    std::vector<Edge> solutionEdges;
    generateMaze(maze, DIM, rng, solutionEdges);

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
    out << "#include <list>\n#include \"maze_structs.h\"\n#include <vector>\n#include <utility>\n\n";
    out << "void generateTrack(std::list<Mazepolygon>& polygons) {\n";
    int idx = 0;
    for (const auto& poly : polygons) {
        out << "    Mazepolygon poly" << idx << ";\n";
        out << "    poly" << idx << ".coordinates = {";
        for (const auto& coord : poly.coordinates) {
            out << "{" << std::fixed << std::setprecision(1) << coord.x << "f, " 
                << coord.y << "f}, ";
        }
        out << "};\n";
        out << "    polygons.push_back(poly" << idx << ");\n";
        idx++;
    }
    out << "}\n";
    out << "void generateSolutionGraph(std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>>& edges) {\n";
    for (const auto& edge : solutionEdges) {
        out << "    edges.push_back({{" << edge.first.first << ", " << edge.first.second << "}, {" << edge.second.first << ", " << edge.second.second << "}});\n";
    }
    out << "}\n";
    out.close();
    std::cout << "SVG-Visualisierung in track3.svg gespeichert.\n";
    std::cout << "Track generiert und in " << output_file << " geschrieben.\n";
    std::cout << "Anzahl der generierten Polygone: " << polygons.size() << "\n";
    return 0;
}