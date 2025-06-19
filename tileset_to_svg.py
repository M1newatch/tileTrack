import json
import os

SVG_SIZE = 400  # Größeres SVG für bessere Sichtbarkeit
GRID_SIZE = 10  # Anzahl der Gitterlinien

with open('tileset.json', 'r', encoding='utf-8') as f:
    tiles = json.load(f)

os.makedirs('svg_tiles', exist_ok=True)

def coord_to_svg(x, y):
    # Skaliere 1x1 auf SVG_SIZE x SVG_SIZE Koordinaten, y invertieren
    return x * SVG_SIZE, SVG_SIZE - y * SVG_SIZE

def create_grid():
    # Erstelle ein Hilfsraster
    grid = []
    step = SVG_SIZE / GRID_SIZE
    
    # Horizontale und vertikale Linien
    for i in range(GRID_SIZE + 1):
        pos = i * step
        # Horizontale Linie
        grid.append(f'<line x1="0" y1="{pos:.1f}" x2="{SVG_SIZE}" y2="{pos:.1f}" stroke="#ddd" stroke-width="1"/>')
        # Vertikale Linie
        grid.append(f'<line x1="{pos:.1f}" y1="0" x2="{pos:.1f}" y2="{SVG_SIZE}" stroke="#ddd" stroke-width="1"/>')
        
        # Beschriftung (nur an den Achsen)
        if i > 0 and i < GRID_SIZE:
            # X-Achse
            grid.append(f'<text x="{pos:.1f}" y="{SVG_SIZE-5}" text-anchor="middle" font-size="12">{i/GRID_SIZE:.1f}</text>')
            # Y-Achse
            grid.append(f'<text x="5" y="{SVG_SIZE-pos:.1f}" font-size="12">{i/GRID_SIZE:.1f}</text>')
    
    return '\n    '.join(grid)

for tile in tiles:
    name = tile['name']
    
    # SVG Header mit hellerem Hintergrund
    svg = f'''<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<svg xmlns="http://www.w3.org/2000/svg" width="{SVG_SIZE}" height="{SVG_SIZE}" viewBox="0 0 {SVG_SIZE} {SVG_SIZE}">
    <!-- Hintergrund -->
    <rect width="100%" height="100%" fill="#f8f8f8"/>
    
    <!-- Hilfsraster -->
    {create_grid()}
    
    <!-- Tile Name -->
    <text x="{SVG_SIZE/2}" y="30" text-anchor="middle" font-size="20" fill="#666">{name}</text>
    
    <!-- Wände -->'''

    # Zeichne jede Wand als gefülltes Polygon
    for wall in tile['walls']:
        svg_coords = [coord_to_svg(x, y) for x, y in wall]
        path = f"M {svg_coords[0][0]:.1f},{svg_coords[0][1]:.1f}"
        for x, y in svg_coords[1:]:
            path += f" L {x:.1f},{y:.1f}"
        path += " Z"  # Pfad schließen
        svg += f'''
    <path d="{path}" fill="#333333" stroke="none"/>'''

    # SVG Footer
    svg += '\n</svg>'
    
    # Speichere SVG
    with open(f'svg_tiles/{name}.svg', 'w', encoding='utf-8') as out:
        out.write(svg)

print("SVG files generated in svg_tiles/ directory") 