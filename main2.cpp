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
#include "maze_structs.h"
#include "json.hpp"
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



using json = nlohmann::json;

constexpr int GRID_SIZE = 10;

struct GridCell {
    bool occupied = false;
    std::string tileName;
    int rotation = 0; // in Grad
    int x = 0, y = 0;
    int from_dir = -1; // 0=right, 1=down, 2=left, 3=up
};

struct Connection {
    std::pair<float, float> entry;
    std::pair<float, float> exit;
    float entry_angle;
    float exit_angle;
};

struct Tile {
    std::string name;
    std::vector<std::vector<std::pair<float, float>>> walls;
    std::vector<Connection> connections;
};

std::vector<Tile> loadTileset(const std::string& filename) {
    std::ifstream file(filename);
    json j;
    file >> j;
    std::vector<Tile> tiles;
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
        if (t.contains("connections")) {
            for (const auto& c : t["connections"]) {
                Connection conn;
                conn.entry = {c["entry"][0], c["entry"][1]};
                conn.exit = {c["exit"][0], c["exit"][1]};
                conn.entry_angle = c["entry_angle"];
                conn.exit_angle = c["exit_angle"];
                tile.connections.push_back(conn);
            }
        }
        tiles.push_back(tile);
    }
    return tiles;
}

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

struct TrackStep {
    int x, y;
    int from_dir; // Richtung aus der das Tile betreten wird
    int to_dir;   // Richtung in die das Tile verlassen wird
    std::string tileName;
    int rotation; // in Grad
    const Connection* conn;
};

// Hilfsfunktion: Nachbarzellen (rechts, unten, links, oben)
std::vector<std::tuple<int, int, int>> getNeighbors(int x, int y) {
    return {
        {x+1, y, 0}, // rechts
        {x, y+1, 1}, // unten
        {x-1, y, 2}, // links
        {x, y-1, 3}  // oben
    };
}

// SVG Export analog zu main.cpp
const int SVG_SIZE = 1000;
const std::string SVG_HEADER =  "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
                                "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\" viewBox=\"%f %f %f %f\">\n"
                                "    <rect width=\"100%%\" height=\"100%%\" fill=\"#f8f8f8\"/>\n";
const std::string SVG_FOOTER = "</svg>";

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

void exportSVG(const std::list<Mazepolygon>& polygons, const std::string& filename) {
    std::ofstream svg(filename);
    Bounds bounds = calculateBounds(polygons);
    float scale = std::min(SVG_SIZE / bounds.width(), SVG_SIZE / bounds.height());
    char header[512];
    snprintf(header, sizeof(header), SVG_HEADER.c_str(), 
             SVG_SIZE, SVG_SIZE, bounds.minX, bounds.minY, bounds.width(), bounds.height());
    svg << header;
    svg << "    <circle cx=\"0\" cy=\"0\" r=\"0.2\" fill=\"red\"/>\n";
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
    svg << SVG_FOOTER;
    svg.close();
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <tileset.json> <output.cpp> [seed]\n";
        return 1;
    }
    std::string tileset_file = argv[1];
    std::string output_file = argv[2];
    unsigned int seed = argc > 3 ? std::stoul(argv[3]) : std::random_device{}();
    std::mt19937 rng(seed);

    auto tiles = loadTileset(tileset_file);
    std::map<std::string, Tile> tileMap;
    for (const auto& tile : tiles) tileMap[tile.name] = tile;

    // 1. Leeres Grid initialisieren
    std::array<std::array<GridCell, GRID_SIZE>, GRID_SIZE> grid{};
    for (int y = 0; y < GRID_SIZE; ++y)
        for (int x = 0; x < GRID_SIZE; ++x) {
            grid[y][x].x = x;
            grid[y][x].y = y;
        }

    // 2. Zufälliges Start-Tile platzieren
    int sx = rng() % GRID_SIZE;
    int sy = rng() % GRID_SIZE;
    int sdir = rng() % 4; // Richtung aus der das Tile betreten wird (0=rechts,1=unten,2=links,3=oben)
    auto& startCell = grid[sy][sx];
    int start_tile_idx = rng() % tiles.size();
    const Tile& start_tile = tiles[start_tile_idx];
    int start_conn_idx = rng() % start_tile.connections.size();
    const Connection* start_conn = &start_tile.connections[start_conn_idx];
    startCell.occupied = true;
    startCell.tileName = start_tile.name;
    startCell.rotation = static_cast<int>(start_conn->entry_angle);
    startCell.from_dir = sdir;

    // 3. Track generieren (Wachstum von der Startzelle aus)
    std::vector<TrackStep> track;
    track.push_back({sx, sy, sdir, -1, start_tile.name, startCell.rotation, start_conn});
    std::set<std::tuple<int, int>> visited;
    visited.insert({sx, sy});
    int max_steps = 200;
    for (int step = 0; step < max_steps; ++step) {
        // Finde alle offenen Enden
        std::vector<TrackStep> open_ends;
        for (const auto& t : track) {
            int tx = t.x, ty = t.y;
            int to_dir = -1;
            if (t.conn) {
                float abs_angle = t.rotation + t.conn->exit_angle;
                int dir = (t.from_dir + static_cast<int>(std::round(t.conn->exit_angle / 90.0f))) % 4;
                int nx = tx, ny = ty;
                if (dir == 0) nx++;
                if (dir == 1) ny++;
                if (dir == 2) nx--;
                if (dir == 3) ny--;
                // Prüfe, ob Zielzelle schon besucht oder belegt ist
                if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE && !grid[ny][nx].occupied && !visited.count({nx, ny})) {
                    open_ends.push_back({nx, ny, (dir+2)%4, -1, "", 0, nullptr});
                }
            }
        }
        if (open_ends.empty()) break;
        // Wähle das zuletzt hinzugefügte offene Ende (Stack-Prinzip)
        auto next_end = open_ends.back();
        int nx = next_end.x, ny = next_end.y, from_dir = next_end.from_dir;
        // Finde ein passendes Tile und eine passende Verbindung
        bool found = false;
        for (int attempt = 0; attempt < 100 && !found; ++attempt) {
            const Tile& cand_tile = tiles[rng() % tiles.size()];
            for (const auto& conn : cand_tile.connections) {
                int conn_dir = static_cast<int>(std::round(conn.entry_angle / 90.0f)) % 4;
                if (conn_dir == from_dir) {
                    // Prüfe erneut, ob Zielzelle frei ist (Redundanz für Sicherheit)
                    if (!grid[ny][nx].occupied && !visited.count({nx, ny})) {
                        grid[ny][nx].occupied = true;
                        grid[ny][nx].tileName = cand_tile.name;
                        grid[ny][nx].rotation = static_cast<int>(conn.entry_angle);
                        grid[ny][nx].from_dir = from_dir;
                        track.push_back({nx, ny, from_dir, -1, cand_tile.name, grid[ny][nx].rotation, &conn});
                        visited.insert({nx, ny});
                        found = true;
                        break;
                    }
                }
            }
        }
        if (!found) break;
    }

    // 4. Polygone generieren
    std::list<Mazepolygon> polygons;
    for (const auto& t : track) {
        const Tile& tile = tileMap[t.tileName];
        float gx = t.x * 1.0f;
        float gy = t.y * 1.0f;
        float angle = t.rotation * M_PI / 180.0f;
        for (const auto& wall : tile.walls) {
            auto global_coords = transform(wall, gx, gy, angle);
            Mazepolygon poly;
            for (const auto& [gx, gy] : global_coords) {
                poly.coordinates.push_back({gx, gy});
            }
            polygons.push_back(poly);
        }
    }

    std::ofstream out(output_file);
    out << "#include <list>\n";
    out << "#include \"maze_structs.h\"\n\n";
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
    out.close();

    std::cout << "Track generiert und in " << output_file << " geschrieben.\n";
    std::cout << "Anzahl der generierten Polygone: " << polygons.size() << "\n";

    // SVG Export
    std::string svg_file = "track2.svg";
    exportSVG(polygons, svg_file);
    std::cout << "SVG-Visualisierung in " << svg_file << " gespeichert.\n";
    return 0;
}
