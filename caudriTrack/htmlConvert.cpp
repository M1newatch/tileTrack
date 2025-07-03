#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>

struct Point {
    float x, y;
};

struct Polygon {
    std::vector<Point> points;
};

struct SolutionPath {
    std::vector<Point> nodes;
};

class HtmlImageMapConverter {
private:
    std::vector<Polygon> polygons;
    std::vector<SolutionPath> solutionPaths;
    
    // Parse coordinates string like "493,729,591,820,644,885,..."
    std::vector<Point> parseCoordinates(const std::string& coordStr) {
        std::vector<Point> points;
        std::stringstream ss(coordStr);
        std::string token;
        std::vector<int> coords;
        
        // Split by comma
        while (std::getline(ss, token, ',')) {
            try {
                coords.push_back(std::stoi(token));
            } catch (const std::exception& e) {
                std::cerr << "Error parsing coordinate: " << token << std::endl;
            }
        }
        
        // Convert pairs of coordinates to points
        for (size_t i = 0; i < coords.size(); i += 2) {
            if (i + 1 < coords.size()) {
                Point p;
                p.x = static_cast<float>(coords[i]) / 100.0f;  // Scale down coordinates
                p.y = static_cast<float>(coords[i + 1]) / 100.0f;
                points.push_back(p);
            }
        }
        
        return points;
    }
    
    // Extract area elements from HTML
    void parseHtmlFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }
        
        std::string line;
        std::regex areaRegex(R"(<area[^>]*coords\s*=\s*["\']([^"\']+)["\'][^>]*shape\s*=\s*["\']([^"\']*)["\'][^>]*>)");
        std::smatch match;
        
        while (std::getline(file, line)) {
            if (std::regex_search(line, match, areaRegex)) {
                std::string coords = match[1].str();
                std::string shape = match[2].str();
                std::vector<Point> points = parseCoordinates(coords);
                
                if (!points.empty()) {
                    if (shape == "poly") {
                        // This is a polygon
                        Polygon poly;
                        poly.points = points;
                        polygons.push_back(poly);
                    } else if (shape.empty()) {
                        // This is a solution path (empty shape)
                        SolutionPath path;
                        path.nodes = points;
                        solutionPaths.push_back(path);
                    }
                }
            }
        }
        
        file.close();
    }
    
    // Generate C++ code
    void generateCppCode(const std::string& outputFilename) {
        std::ofstream outFile(outputFilename);
        if (!outFile.is_open()) {
            std::cerr << "Error: Could not create output file " << outputFilename << std::endl;
            return;
        }
        
        // Write includes and function signature
        outFile << "#include <iostream>\n";
        outFile << "#include <list>\n";
        outFile << "#include <cmath>\n";
        outFile << "#include <vector>\n";
        outFile << "#include <algorithm>\n\n";
        outFile << "#include \"maze_generator.h\"\n\n";
        
        outFile << "void MazeGenerator::generateMaze()\n{\n";
        
        // Generate polygons
        for (size_t i = 0; i < polygons.size(); ++i) {
            outFile << "    // Polygon " << (i + 1) << "\n";
            outFile << "    std::vector<MazeCoordinate> coords" << (i + 1) << " = {\n";
            
            for (size_t j = 0; j < polygons[i].points.size(); ++j) {
                outFile << "        {" << polygons[i].points[j].x << "f, " 
                        << polygons[i].points[j].y << "f}";
                
                if (j == 0) outFile << ",  // start";
                else if (j == polygons[i].points.size() - 1) outFile << "   // end";
                else outFile << ",";
                
                outFile << "\n";
            }
            
            outFile << "    };\n\n";
            
            outFile << "    Mazepolygon poly" << (i + 1) << ";\n";
            outFile << "    for (const auto& coord : coords" << (i + 1) << ") {\n";
            outFile << "        poly" << (i + 1) << ".coordinates.push_back(coord);\n";
            outFile << "    }\n\n";
            outFile << "    polygons.push_back(poly" << (i + 1) << ");\n\n";
        }
        
        // Generate solution graph nodes
        int nodeCounter = 1;
        for (size_t pathIdx = 0; pathIdx < solutionPaths.size(); ++pathIdx) {
            outFile << "    // Solution path " << (pathIdx + 1) << " nodes\n";
            
            std::vector<int> pathNodeNumbers;
            
            // Create all nodes for this path
            for (size_t nodeIdx = 0; nodeIdx < solutionPaths[pathIdx].nodes.size(); ++nodeIdx) {
                const Point& node = solutionPaths[pathIdx].nodes[nodeIdx];
                
                outFile << "    SolutionGraphNode node" << nodeCounter << ";\n";
                outFile << "    node" << nodeCounter << ".coordinate = {" 
                        << node.x << "f, " << node.y << "f};\n";
                
                pathNodeNumbers.push_back(nodeCounter);
                nodeCounter++;
            }
            
            outFile << "\n";
            
            // Add all nodes to the solution graph
            for (int nodeNum : pathNodeNumbers) {
                outFile << "    solutionGraph.push_back(node" << nodeNum << ");\n";
            }
            
            outFile << "\n";
            
            // Set up neighbors for each node in the path (continuous loop)
            for (size_t nodeIdx = 0; nodeIdx < pathNodeNumbers.size(); ++nodeIdx) {
                int currentNodeNum = pathNodeNumbers[nodeIdx];
                
                outFile << "    // Setting neighbors for node" << currentNodeNum << "\n";
                
                // Add previous node as neighbor (with loop wrapping)
                size_t prevIdx = (nodeIdx == 0) ? pathNodeNumbers.size() - 1 : nodeIdx - 1;
                int prevNodeNum = pathNodeNumbers[prevIdx];
                outFile << "    node" << currentNodeNum 
                        << ".neighbors.push_back(&node" << prevNodeNum << ");\n";
                
                // Add next node as neighbor (with loop wrapping)
                size_t nextIdx = (nodeIdx == pathNodeNumbers.size() - 1) ? 0 : nodeIdx + 1;
                int nextNodeNum = pathNodeNumbers[nextIdx];
                outFile << "    node" << currentNodeNum 
                        << ".neighbors.push_back(&node" << nextNodeNum << ");\n";
            }
            
            outFile << "\n";
        }
        
        outFile << "}\n";
        outFile.close();
        
        std::cout << "Generated C++ code with " << polygons.size() << " polygons and " 
                  << solutionPaths.size() << " solution paths in " << outputFilename << std::endl;
    }
    
public:
    void convert(const std::string& htmlFile, const std::string& cppFile) {
        std::cout << "Converting HTML image map from " << htmlFile << " to " << cppFile << std::endl;
        
        parseHtmlFile(htmlFile);
        generateCppCode(cppFile);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <input.html> <output.cpp>" << std::endl;
        std::cout << "Example: " << argv[0] << " track.html track_generated.cpp" << std::endl;
        return 1;
    }
    
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    
    HtmlImageMapConverter converter;
    converter.convert(inputFile, outputFile);
    
    return 0;
}