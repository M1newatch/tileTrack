import json
import os

SVG_SIZE = 400
GRID_SIZE = 10  # number of grid cells

with open('tileset.json', 'r', encoding='utf-8') as f:
    tiles = json.load(f)

os.makedirs('svg_tiles', exist_ok=True)

def coord_to_svg(x, y):
    # Scale 1x1 to SVG_SIZE x SVG_SIZE coordinates, invert y
    return x * SVG_SIZE, SVG_SIZE - y * SVG_SIZE

def create_grid():
    grid = []
    step = SVG_SIZE / GRID_SIZE
    
    # gridlines
    for i in range(GRID_SIZE + 1):
        pos = i * step
        grid.append(f'<line x1="0" y1="{pos:.1f}" x2="{SVG_SIZE}" y2="{pos:.1f}" stroke="#ddd" stroke-width="1"/>')
        grid.append(f'<line x1="{pos:.1f}" y1="0" x2="{pos:.1f}" y2="{SVG_SIZE}" stroke="#ddd" stroke-width="1"/>')
        
        # axis labeling
        if i > 0 and i < GRID_SIZE:
            # X-axis
            grid.append(f'<text x="{pos:.1f}" y="{SVG_SIZE-5}" text-anchor="middle" font-size="12">{i/GRID_SIZE:.1f}</text>')
            # Y-axis
            grid.append(f'<text x="5" y="{SVG_SIZE-pos:.1f}" font-size="12">{i/GRID_SIZE:.1f}</text>')
    
    return '\n    '.join(grid)

for tile in tiles:
    name = tile['name']
    
    # SVG header with empty background
    svg = f'''<?xml version="1.0" encoding="UTF-8" standalone="no"?>
                <svg xmlns="http://www.w3.org/2000/svg" width="{SVG_SIZE}" height="{SVG_SIZE}" viewBox="0 0 {SVG_SIZE} {SVG_SIZE}">
                <!-- Hintergrund -->
                <rect width="100%" height="100%" fill="#f8f8f8"/>
    
                <!-- Hilfsraster -->
                {create_grid()}
                
                <!-- Tile Name -->
                <text x="{SVG_SIZE/2}" y="30" text-anchor="middle" font-size="20" fill="#666">{name}</text>
                
                <!-- Wände -->'''

    # walls as closed polygons
    for wall in tile['walls']:
        svg_coords = [coord_to_svg(x, y) for x, y in wall]
        path = f"M {svg_coords[0][0]:.1f},{svg_coords[0][1]:.1f}"
        for x, y in svg_coords[1:]:
            path += f" L {x:.1f},{y:.1f}"
        path += " Z"
        svg += f'''
    <path d="{path}" fill="#333333" stroke="none"/>'''

    # SVG footer
    svg += '\n</svg>'
    
    with open(f'svg_tiles/{name}.svg', 'w', encoding='utf-8') as out:
        out.write(svg)

print("SVG files generated in svg_tiles/ directory")