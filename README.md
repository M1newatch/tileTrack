## Setup Project
---

## Build instructions
---
```bash
g++ -std=c++17 main.cpp -o trackgen 
```

## Run instructions
---
```bash
./trackgen <tileset.json> <output.cpp> [grid_size] [seed]
./trackgen tileset.json output.cpp 10 
```
seed parameter can be omittet for random seed

## SVG visualisation
To visualize the .json tileset without having to run a simulation you can render a SVG representation of the maze with this command.
```bash
python tileset_to_svg.py
```

s