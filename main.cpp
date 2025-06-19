#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <random>
#include <string>
#include <cmath>
#include <math.h>
#include <iomanip>
#include <sstream>
#include "maze_structs.h"
#include "json.hpp" // nlohmann/json
#include <set>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <array>
#include <stack>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using json = nlohmann::json;

// SVG Konstanten
const int SVG_SIZE = 1000;
const std::string SVG_HEADER =  "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
                                "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\" viewBox=\"%f %f %f %f\">\n"
                                "    <rect width=\"100%%\" height=\"100%%\" fill=\"#f8f8f8\"/>\n";
const std::string SVG_FOOTER = "</svg>";

// Struktur für SVG-Bounds
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
    float centerX() const { return (minX + maxX) / 2; }
    float centerY() const { return (minY + maxY) / 2; }
};

// Bestimmt die Bounds aller Polygone
Bounds calculateBounds(const std::list<Mazepolygon>& polygons) {
    Bounds bounds;
    for (const auto& poly : polygons) {
        for (const auto& coord : poly.coordinates) {
            bounds.update(coord.x, coord.y);
        }
    }
    // Füge etwas Padding hinzu
    float padding = 0.1f;
    bounds.minX -= padding;
    bounds.minY -= padding;
    bounds.maxX += padding;
    bounds.maxY += padding;
    return bounds;
}

// Exportiert die Strecke als SVG
void exportSVG(const std::list<Mazepolygon>& polygons, const std::string& filename) {
    std::ofstream svg(filename);
    
    // Berechne Bounds
    Bounds bounds = calculateBounds(polygons);
    float scale = std::min(SVG_SIZE / bounds.width(), SVG_SIZE / bounds.height());
    
    // Schreibe SVG Header mit berechneten Dimensionen
    char header[512];
    snprintf(header, sizeof(header), SVG_HEADER.c_str(), 
             SVG_SIZE, SVG_SIZE, bounds.minX, bounds.minY, bounds.width(), bounds.height());
    svg << header;
    
    // Zeichne Start/Ziel-Markierung
    svg << "    <circle cx=\"0\" cy=\"0\" r=\"0.2\" fill=\"red\"/>\n";
    
    // Zeichne jedes Polygon
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
    
    // Zeichne Koordinatensystem
    std::stringstream axes;
    axes << "    <g stroke=\"#999\" stroke-width=\"0.02\">\n"
         << "        <line x1=\"" << bounds.minX << "\" y1=\"0\" x2=\"" << bounds.maxX << "\" y2=\"0\"/>\n"
         << "        <line x1=\"0\" y1=\"" << bounds.minY << "\" x2=\"0\" y2=\"" << bounds.maxY << "\"/>\n"
         << "    </g>\n";
    svg << axes.str();
    
    svg << SVG_FOOTER;
    svg.close();
}

// Helper to normalize angles to [0, 2*PI)
float normalize_angle(float angle) {
    angle = std::fmod(angle, 2 * M_PI);
    if (angle < 0) angle += 2 * M_PI;
    return angle;
}

// Define a Connection struct for entry/exit/angle
struct Connection {
    std::pair<float, float> entry;
    std::pair<float, float> exit;
    float entry_angle; // in degrees
    float exit_angle;  // in degrees
};

struct Tile {
    std::string name;
    std::vector<std::vector<std::pair<float, float>>> walls;
    std::vector<Connection> connections; // Multiple possible connections
};

struct Position {
    float x;
    float y;
    float angle;  // in Radians
};

// Berechnet die Distanz zwischen zwei Punkten
float distance(float x1, float y1, float x2, float y2) {
    return std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

// Prüft, ob wir eine gültige Schließung der Strecke haben
bool canClosePath(const Position& start, const Position& current, float threshold = 1.0f) {
    // Prüfe Distanz zum Startpunkt
    float dist = distance(start.x, start.y, current.x, current.y);
    
    // Prüfe ob der Winkel ungefähr passt (erlaubt 90-Grad-Schritte)
    float angleDiff = std::fmod(std::abs(current.angle - start.angle), 2 * M_PI);
    bool angleAligned = angleDiff < 0.1f || std::abs(angleDiff - M_PI/2) < 0.1f || 
                       std::abs(angleDiff - M_PI) < 0.1f || std::abs(angleDiff - 3*M_PI/2) < 0.1f;
    
    return dist < threshold && angleAligned;
}

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
        // Load connections
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

std::vector<std::pair<float, float>> transform(
    const std::vector<std::pair<float, float>>& coords,
    float dx, float dy, float angle_rad
) {
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

// Wählt ein passendes Tile basierend auf der aktuellen Position und dem Startpunkt
std::string selectNextTile(const Position& start, const Position& current, int stepCount, std::mt19937& rng) {
    float dist = distance(start.x, start.y, current.x, current.y);
    
    // Wenn wir gerade gestartet sind (erste 3 Schritte), nur Geraden oder Kurven
    if (stepCount < 3) {
        if (rng() % 2 == 0) {
            return "straight";
        } else {
            // Wähle eine zufällige Kurvenrichtung
            int curve_type = rng() % 4;
            switch(curve_type) {
                case 0: return "curve_90_south_west";
                case 1: return "curve_90_south_east";
                case 2: return "curve_90_north_east";
                default: return "curve_90_north_west";
            }
        }
    }
    
    // Wenn wir nah am Start sind und mindestens 6 Schritte gemacht haben, versuche zu schließen
    if (stepCount >= 6 && dist < 2.0f) {
        // Wähle die passende Kurve basierend auf der aktuellen Richtung
        float angle_to_start = std::atan2(start.y - current.y, start.x - current.x) * 180.0f / M_PI;
        float current_angle = current.angle * 180.0f / M_PI;
        
        // Normalisiere die Winkel
        while (angle_to_start < 0) angle_to_start += 360.0f;
        while (current_angle < 0) current_angle += 360.0f;
        
        // Bestimme die benötigte Drehrichtung
        float angle_diff = angle_to_start - current_angle;
        while (angle_diff < -180.0f) angle_diff += 360.0f;
        while (angle_diff > 180.0f) angle_diff -= 360.0f;
        
        // Wähle die passende Kurve basierend auf der aktuellen Richtung und der benötigten Drehung
        if (current_angle < 45.0f || current_angle > 315.0f) { // Facing right
            return (angle_diff > 0) ? "curve_90_north_east" : "curve_90_south_east";
        } else if (current_angle < 135.0f) { // Facing down
            return (angle_diff > 0) ? "curve_90_south_east" : "curve_90_south_west";
        } else if (current_angle < 225.0f) { // Facing left
            return (angle_diff > 0) ? "curve_90_south_west" : "curve_90_north_west";
        } else { // Facing up
            return (angle_diff > 0) ? "curve_90_north_west" : "curve_90_north_east";
        }
    }
    
    // Normale Streckengeneration
    int r = rng() % 100;
    if (r < 40) return "straight";     // 40% Chance für Gerade
    if (r < 90) {                      // 50% Chance für Kurve
        // Wähle eine zufällige Kurvenrichtung
        int curve_type = rng() % 4;
        switch(curve_type) {
            case 0: return "curve_90_south_west";
            case 1: return "curve_90_south_east";
            case 2: return "curve_90_north_east";
            default: return "curve_90_north_west";
        }
    }
    if (r < 95) return "t_junction";   // 5% Chance für T-Kreuzung
    return "cross";                    // 5% Chance für Kreuzung
}

struct TrackState {
    float x = 0.0f;
    float y = 0.0f;
    float angle = 0.0f;  // in Grad
    
    void move(float distance) {
        float radians = angle * M_PI / 180.0f;
        x += distance * cos(radians);
        y += distance * sin(radians);
    }
    
    void rotate(float degrees) {
        angle = fmod(angle + degrees, 360.0f);
        if (angle < 0) angle += 360.0f;
    }
};

// Transformiert ein Polygon basierend auf Position und Rotation
Mazepolygon transformPolygon(const Mazepolygon& original, const TrackState& state) {
    Mazepolygon transformed = original;
    float radians = state.angle * M_PI / 180.0f;
    float cos_angle = cos(radians);
    float sin_angle = sin(radians);
    
    for (auto& coord : transformed.coordinates) {
        float old_x = coord.x;
        float old_y = coord.y;
        
        // Rotation um den Ursprung
        coord.x = old_x * cos_angle - old_y * sin_angle;
        coord.y = old_x * sin_angle + old_y * cos_angle;
        
        // Translation
        coord.x += state.x;
        coord.y += state.y;
    }
    
    return transformed;
}

// Generiert eine geschlossene Strecke
std::list<Mazepolygon> generateTrack(const std::vector<Tile>& tiles) {
    std::list<Mazepolygon> polygons;
    TrackState state;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> rotDist(0, 360);
    
    float tileSize = 1.0f;  // Größe eines Tiles
    int minTiles = 6;       // Mindestanzahl an Tiles
    int maxAttempts = 1000; // Maximale Versuche
    int currentTiles = 0;
    
    // Wahrscheinlichkeitsverteilung für Tile-Typen
    std::discrete_distribution<> tileDist({40, 50, 5, 5}); // Gerade, Kurve, T, Kreuzung
    
    // Track occupied positions (rounded to 2 decimals for grid)
    std::set<std::tuple<int, int, int>> occupied;
    // Mark the start position as occupied
    occupied.insert({static_cast<int>(std::round(state.x * 100)), static_cast<int>(std::round(state.y * 100)), static_cast<int>(std::round(normalize_angle(state.angle) * 1000))});
    
    while (currentTiles < maxAttempts) {
        // Prüfe, ob wir die Strecke schließen können
        float distToStart = sqrt(state.x * state.x + state.y * state.y);
        if (currentTiles >= minTiles && distToStart <= tileSize * 1.5f) {
            // Versuche die Strecke zu schließen
            float angleToStart = atan2(-state.y, -state.x) * 180.0f / M_PI;
            float angleDiff = fmod(angleToStart - state.angle + 540.0f, 360.0f) - 180.0f;
            
            if (abs(angleDiff) <= 45.0f) {
                // Wir können die Strecke schließen
                state.angle = angleToStart;
                state.move(distToStart);
                break;
            }
        }
        
        // Wähle zufälliges Tile basierend auf Wahrscheinlichkeiten
        int tileIndex = tileDist(gen);
        const Tile& selectedTile = tiles[tileIndex];
        
        // Find a connection whose entry matches the current approach (within epsilon for both position and angle)
        const Connection* chosen_conn = nullptr;
        for (const auto& conn : selectedTile.connections) {
            // Transform the entry point of the candidate connection to global coordinates
            float entry_angle_rad = conn.entry_angle * M_PI / 180.0f;
            float cos_a = std::cos(state.angle);
            float sin_a = std::sin(state.angle);
            float local_entry_x = conn.entry.first;
            float local_entry_y = conn.entry.second;
            float global_entry_x = cos_a * local_entry_x - sin_a * local_entry_y + state.x;
            float global_entry_y = sin_a * local_entry_x + cos_a * local_entry_y + state.y;
            // The entry point of the new tile must match the current position
            float entry_dist = distance(global_entry_x, global_entry_y, state.x, state.y);
            // The entry angle of the new tile must match the current angle
            float angle_diff = std::abs(normalize_angle(state.angle) - normalize_angle(entry_angle_rad));
            if (entry_dist < 0.001 && angle_diff < 0.01) {
                chosen_conn = &conn;
                break;
            }
        }
        if (!chosen_conn && !selectedTile.connections.empty()) {
            chosen_conn = &selectedTile.connections[0];
        }

        // Check for overlap: compute the new tile's grid position and angle
        float cos_a = std::cos(state.angle);
        float sin_a = std::sin(state.angle);
        float local_exit_x = chosen_conn->exit.first;
        float local_exit_y = chosen_conn->exit.second;
        float global_exit_x = cos_a * local_exit_x - sin_a * local_exit_y + state.x;
        float global_exit_y = sin_a * local_exit_x + cos_a * local_exit_y + state.y;
        float new_angle = normalize_angle(state.angle + chosen_conn->exit_angle * M_PI / 180.0f);
        // Auch Entry-Position prüfen
        float local_entry_x = chosen_conn->entry.first;
        float local_entry_y = chosen_conn->entry.second;
        float global_entry_x = cos_a * local_entry_x - sin_a * local_entry_y + state.x;
        float global_entry_y = sin_a * local_entry_x + cos_a * local_entry_y + state.y;
        auto occ_key_exit = std::make_tuple(static_cast<int>(std::round(global_exit_x * 100)), static_cast<int>(std::round(global_exit_y * 100)), static_cast<int>(std::round(new_angle * 1000)));
        auto occ_key_entry = std::make_tuple(static_cast<int>(std::round(global_entry_x * 100)), static_cast<int>(std::round(global_entry_y * 100)), static_cast<int>(std::round(normalize_angle(state.angle) * 1000)));
        if (occupied.count(occ_key_exit) || occupied.count(occ_key_entry)) {
            std::cout << "Overlap detected at (" << global_exit_x << ", " << global_exit_y << ", " << new_angle << ") or entry (" << global_entry_x << ", " << global_entry_y << "). Skipping tile.\n";
            break;
        }
        occupied.insert(occ_key_exit);
        occupied.insert(occ_key_entry);

        // Debug output: step, tile, connection, position, angle
        std::cout << "Step " << currentTiles << ": Tile '" << selectedTile.name << "'\n";
        std::cout << "  Entry: (" << chosen_conn->entry.first << ", " << chosen_conn->entry.second << ")  Exit: (" << chosen_conn->exit.first << ", " << chosen_conn->exit.second << ")  Entry angle: " << chosen_conn->entry_angle << " Exit angle: " << chosen_conn->exit_angle << "\n";
        std::cout << "  Current pos: (" << state.x << ", " << state.y << ")  Angle: " << state.angle * 180.0 / M_PI << " deg\n";

        // Transform and add all polygons of the tile
        for (const auto& wall : selectedTile.walls) {
            auto global_coords = transform(wall, state.x, state.y, state.angle * M_PI / 180.0f);
            Mazepolygon transformedPoly;
            for (const auto& [gx, gy] : global_coords) {
                transformedPoly.coordinates.push_back({gx, gy});
            }
            polygons.push_back(transformedPoly);
        }

        // Position für nächstes Tile aktualisieren
        state.x = global_exit_x;
        state.y = global_exit_y;
        state.angle = new_angle;
        
        currentTiles++;
    }
    
    return polygons;
}

struct AStarNode {
    Position pos;
    int steps;
    float cost;
    float priority;
    std::vector<std::string> tile_names;
    std::vector<const Connection*> connections;
    // Für Rekonstruktion
    std::shared_ptr<AStarNode> parent;
    bool operator>(const AStarNode& other) const { return priority > other.priority; }
};

float heuristic(const Position& from, const Position& to) {
    float dist = distance(from.x, from.y, to.x, to.y);
    float angleDiff = std::abs(normalize_angle(from.angle) - normalize_angle(to.angle));
    return dist + angleDiff;
}

// Hash für Position (x, y, angle)
struct PosHash {
    std::size_t operator()(const std::tuple<int, int, int>& t) const {
        return std::hash<int>()(std::get<0>(t)) ^ std::hash<int>()(std::get<1>(t)) ^ std::hash<int>()(std::get<2>(t));
    }
};

// A*-basierte Track-Generierung
std::vector<std::pair<std::string, const Connection*>> generateTrackAStar(const std::map<std::string, Tile>& tileMap, const Position& start, int maxTiles) {
    using NodePtr = std::shared_ptr<AStarNode>;
    std::priority_queue<NodePtr, std::vector<NodePtr>, std::function<bool(const NodePtr&, const NodePtr&)>> openSet(
        [](const NodePtr& a, const NodePtr& b) { return a->priority > b->priority; });
    std::unordered_map<std::tuple<int, int, int>, float, PosHash> closedSet;

    NodePtr startNode = std::make_shared<AStarNode>();
    startNode->pos = start;
    startNode->steps = 0;
    startNode->cost = 0;
    startNode->priority = 0;
    openSet.push(startNode);

    while (!openSet.empty()) {
        NodePtr current = openSet.top(); openSet.pop();
        auto occ_key = std::make_tuple(static_cast<int>(std::round(current->pos.x * 100)), static_cast<int>(std::round(current->pos.y * 100)), static_cast<int>(std::round(normalize_angle(current->pos.angle) * 1000)));
        if (closedSet.count(occ_key) && closedSet[occ_key] <= current->cost) continue;
        closedSet[occ_key] = current->cost;

        // Zielbedingung: zurück am Start (mit gleichem Winkel), mind. 6 Schritte
        if (current->steps >= 6 && distance(current->pos.x, current->pos.y, start.x, start.y) < 0.01 && std::abs(normalize_angle(current->pos.angle) - normalize_angle(start.angle)) < 0.01) {
            // Rekonstruiere Pfad
            std::vector<std::pair<std::string, const Connection*>> path;
            NodePtr node = current;
            while (node->parent) {
                path.push_back({node->tile_names.back(), node->connections.back()});
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        if (current->steps >= maxTiles) continue;

        // Für alle Tiles prüfen, ob sie anschließbar sind
        for (const auto& [tileName, tile] : tileMap) {
            for (const auto& conn : tile.connections) {
                // Prüfe, ob Entry-Punkt und Entry-Winkel passen
                float entry_angle_rad = conn.entry_angle * M_PI / 180.0f;
                float cos_a = std::cos(current->pos.angle);
                float sin_a = std::sin(current->pos.angle);
                float local_entry_x = conn.entry.first;
                float local_entry_y = conn.entry.second;
                float global_entry_x = cos_a * local_entry_x - sin_a * local_entry_y + current->pos.x;
                float global_entry_y = sin_a * local_entry_x + cos_a * local_entry_y + current->pos.y;
                float entry_dist = distance(global_entry_x, global_entry_y, current->pos.x, current->pos.y);
                float angle_diff = std::abs(normalize_angle(current->pos.angle) - normalize_angle(entry_angle_rad));
                if (entry_dist < 0.001 && angle_diff < 0.01) {
                    // Berechne neue Position nach diesem Tile
                    float local_exit_x = conn.exit.first;
                    float local_exit_y = conn.exit.second;
                    float global_exit_x = cos_a * local_exit_x - sin_a * local_exit_y + current->pos.x;
                    float global_exit_y = sin_a * local_exit_x + cos_a * local_exit_y + current->pos.y;
                    float new_angle = normalize_angle(current->pos.angle + conn.exit_angle * M_PI / 180.0f);
                    auto occ_key_next = std::make_tuple(static_cast<int>(std::round(global_exit_x * 100)), static_cast<int>(std::round(global_exit_y * 100)), static_cast<int>(std::round(new_angle * 1000)));
                    if (closedSet.count(occ_key_next)) continue; // Schon besucht
                    // Erzeuge neuen Knoten
                    NodePtr next = std::make_shared<AStarNode>();
                    next->pos = {global_exit_x, global_exit_y, new_angle};
                    next->steps = current->steps + 1;
                    next->cost = current->cost + 1;
                    next->priority = next->cost + heuristic(next->pos, start);
                    next->tile_names = current->tile_names; next->tile_names.push_back(tileName);
                    next->connections = current->connections; next->connections.push_back(&conn);
                    next->parent = current;
                    openSet.push(next);
                }
            }
        }
    }
    // Kein Pfad gefunden
    return {};
}

constexpr int GRID_SIZE = 15;
constexpr int MAX_TILES = GRID_SIZE * GRID_SIZE;

struct GridCell {
    bool inPath = false;
    int x = 0, y = 0;
    float angle = 0.0f;
    std::string tileName;
    const Connection* conn = nullptr;
};

// Hilfsfunktionen für Nachbarn
std::vector<std::pair<int, int>> getNeighbors(int x, int y, std::mt19937& rng) {
    std::vector<std::pair<int, int>> n = {{x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}};
    std::shuffle(n.begin(), n.end(), rng);
    return n;
}

// Maze-Backtracking für einfachen Pfad (kein Loop)
bool generateSimplePath(std::array<std::array<GridCell, GRID_SIZE>, GRID_SIZE>& grid, int startX, int startY, int& pathLen, int& endX, int& endY, std::mt19937& rng) {
    std::stack<std::tuple<int, int, float>> stack;
    stack.push({startX, startY, 0.0f});
    grid[startY][startX].inPath = true;
    pathLen = 1;
    int maxLen = 1;
    int bestX = startX, bestY = startY;
    while (!stack.empty() && pathLen < MAX_TILES) {
        auto [x, y, angle] = stack.top();
        stack.pop();
        std::vector<std::pair<int, int>> neighbors = getNeighbors(x, y, rng);
        bool extended = false;
        for (auto [nx, ny] : neighbors) {
            if (nx >= 0 && nx < GRID_SIZE && ny >= 0 && ny < GRID_SIZE && !grid[ny][nx].inPath) {
                grid[ny][nx].inPath = true;
                grid[ny][nx].x = nx;
                grid[ny][nx].y = ny;
                grid[ny][nx].angle = angle;
                pathLen++;
                stack.push({nx, ny, angle});
                extended = true;
                if (pathLen > maxLen) {
                    maxLen = pathLen;
                    bestX = nx;
                    bestY = ny;
                }
                break;
            }
        }
        if (!extended) {
            // Dead end, backtrack
            pathLen--;
        }
    }
    endX = bestX;
    endY = bestY;
    return true;
}

// Tile-Auswahl für Pfad
std::string chooseTileForCell(const std::array<std::array<GridCell, GRID_SIZE>, GRID_SIZE>& grid, int x, int y) {
    // Zähle Nachbarn im Pfad
    int count = 0;
    bool up = false, down = false, left = false, right = false;
    if (y > 0 && grid[y-1][x].inPath) { count++; up = true; }
    if (y < GRID_SIZE-1 && grid[y+1][x].inPath) { count++; down = true; }
    if (x > 0 && grid[y][x-1].inPath) { count++; left = true; }
    if (x < GRID_SIZE-1 && grid[y][x+1].inPath) { count++; right = true; }
    if (count == 2) {
        if ((up && down) || (left && right)) return "straight";
        if (up && right) return "curve_90_north_east";
        if (up && left) return "curve_90_north_west";
        if (down && right) return "curve_90_south_east";
        if (down && left) return "curve_90_south_west";
    } else if (count == 3) {
        return "t_junction";
    } else if (count == 4) {
        return "cross";
    }
    return "straight"; // Fallback
}

// Bestimme die Rotation für ein Tile anhand der Nachbarschaft
float determineTileAngle(const std::array<std::array<GridCell, GRID_SIZE>, GRID_SIZE>& grid, int x, int y, const std::string& tileName) {
    bool up = (y > 0 && grid[y-1][x].inPath);
    bool down = (y < GRID_SIZE-1 && grid[y+1][x].inPath);
    bool left = (x > 0 && grid[y][x-1].inPath);
    bool right = (x < GRID_SIZE-1 && grid[y][x+1].inPath);
    if (tileName == "straight") {
        if (up && down) return 90.0f;
        if (left && right) return 0.0f;
    } else if (tileName == "curve_90_north_east") {
        if (up && right) return 0.0f;
        if (right && down) return 90.0f;
        if (down && left) return 180.0f;
        if (left && up) return 270.0f;
    } else if (tileName == "curve_90_north_west") {
        if (up && left) return 0.0f;
        if (left && down) return 90.0f;
        if (down && right) return 180.0f;
        if (right && up) return 270.0f;
    } else if (tileName == "curve_90_south_east") {
        if (down && right) return 0.0f;
        if (right && up) return 90.0f;
        if (up && left) return 180.0f;
        if (left && down) return 270.0f;
    } else if (tileName == "curve_90_south_west") {
        if (down && left) return 0.0f;
        if (left && up) return 90.0f;
        if (up && right) return 180.0f;
        if (right && down) return 270.0f;
    } else if (tileName == "t_junction") {
        // Öffnung zeigt zur Seite ohne Nachbarn
        if (!up) return 270.0f;
        if (!right) return 0.0f;
        if (!down) return 90.0f;
        if (!left) return 180.0f;
    } else if (tileName == "cross") {
        return 0.0f;
    }
    return 0.0f; // Fallback
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <tileset.json> <output.cpp> [seed]\n";
        return 1;
    }
    std::string tileset_file = argv[1];
    std::string output_file = argv[2];
    unsigned int seed = argc > 3 ? std::stoul(argv[3]) : std::random_device{}();

    auto tiles = loadTileset(tileset_file);
    std::mt19937 rng(seed);

    // Map für schnellen Tile-Zugriff
    std::map<std::string, Tile> tileMap;
    for (const auto& tile : tiles) {
        tileMap[tile.name] = tile;
    }
    // Grid initialisieren
    std::array<std::array<GridCell, GRID_SIZE>, GRID_SIZE> grid{};
    int startX = GRID_SIZE / 2, startY = GRID_SIZE / 2;
    int pathLen = 0;
    int endX = startX, endY = startY;
    // Erzeuge einfachen Pfad (kein Loop)
    bool pathOk = generateSimplePath(grid, startX, startY, pathLen, endX, endY, rng);
    if (!pathOk) {
        std::cout << "Konnte keinen Pfad im Grid erzeugen!\n";
        return 1;
    }
    // Tiles im gesamten Grid setzen
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            if (grid[y][x].inPath) {
                grid[y][x].tileName = chooseTileForCell(grid, x, y);
                grid[y][x].angle = determineTileAngle(grid, x, y, grid[y][x].tileName);
            } else {
                grid[y][x].tileName = "block";
                grid[y][x].angle = 0.0f;
            }
        }
    }
    // Polygone generieren
    std::list<Mazepolygon> polygons;
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            if (grid[y][x].inPath) {
                const Tile& tile = tileMap[grid[y][x].tileName];
                float gx = x * 1.0f;
                float gy = y * 1.0f;
                float angle = grid[y][x].angle;
                for (const auto& wall : tile.walls) {
                    auto global_coords = transform(wall, gx, gy, angle * M_PI / 180.0f);
                    Mazepolygon poly;
                    for (const auto& [gx, gy] : global_coords) {
                        poly.coordinates.push_back({gx, gy});
                    }
                    polygons.push_back(poly);
                }
            }
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

    std::cout << "Strecke generiert und in " << output_file << " geschrieben.\n";
    std::cout << "Anzahl der generierten Polygone: " << idx << "\n";

    // Nach der Streckengenerierung, exportiere auch als SVG
    std::string svg_file = "track.svg";
    exportSVG(polygons, svg_file);
    std::cout << "SVG-Visualisierung in " << svg_file << " gespeichert.\n";

    return 0;
} 