# Tile-based Maze Generation + Caudri Track

A C++ project for generating procedural mazes using depth first searcha with tile-based constraints. Features HTML image map conversion tools for importing custom track layouts and generating polygon-based maze structures with solution paths. The output files this programm
produces are meant to be used with the Gazebo simulation software.

## Features

- **Procedural Generation**: Creates random mazes using depth-first search with configurable crossings
- **Tile-based System**: Uses JSON tileset definitions with rotation and edge constraints
- **HTML Converter**: Converts HTML image maps to C++ polygon structures for custom track designs
- **SVG Visualization**: Generates visual representations of generated tracks and mazes
- **Solution Graph**: Creates connected waypoint networks for AI pathfinding

## Setup Project
1. Dowload the repository from Github: https://github.com/M1newatch/tileTrack.
2. For procedural maze generation change to the src directory, for converting a custom track go to the caurdiTrack directory.
3. This project requires <nlohmann/json.hpp> library: https://github.com/nlohmann/json

## Build instructions
**maze generation:**
```bash
g++ -std=c++17 main.cpp -o trackgen 
```

**custom track:**
```bash
g++ htmlConvert.cpp -o htmlConvert
```

## CLI Parameters
**maze generation:**
```bash
./trackgen <tileset.json> <output.cpp> [grid size] [crossing probability] [seed]
```
The following parameters can be omitted:

- [grid size] 
- [crossing probability] 
- [seed]

The programm will default to:

- grid size = 10
- crossing probability = 0.1
- seed = random

**custom track:**
```
./htmlConvert.exe <input.html> <output.cpp>
```
The input html file should contain a html image map for this program to work. The file can be generated with the help of the following website: https://www.image-map.net/

## Example Execution
**maze generation:**
```
./trackgen tileset.json output.cpp 10 0.1
```

To visualize the .json tileset without having to run a simulation you can render a SVG representation of the maze with this command.
```bash
python tileset_to_svg.py
```

**custom track:**
```
./htmlConvert.exe track.html track_generated.cpp
```

## Expected output
**maze generation:**
- SVG representation saved in track.svg
- Track generated and saved in output.cpp
- Number of generated polygons: [num]

**svg conversion**
- SVG files generated in svg_tiles/ directory

**custom track:**
- Converting HTML image map from track.html to track_generated.cpp
- Generated C++ code with 4 polygons and 1 solution paths in track_generated.cpp