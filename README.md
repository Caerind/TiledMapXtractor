# TiledMapXtractor (TMX)

Library for .tmx map with SFML

## Features

- Loading
- Saving
- Modification
- Orthogonal, Isometric, Staggered and Hexagonal (both with support for Axis and Index)
- Most objects are supported (not concave polygons and flipped tiles)
- External tileset (.tsx)
- Almost all .tmx data (Please use the issue tracker if your output isn't the same as your tiled editor)

## What is not supported / TODO

As the library is young, most features will be added soon

- Multiple tilesets for rendering (only one tileset per layer)
- Tileset:Tile:ObjectGroup
- Tileset:Tile:Image
- Image:Data
- Compression/Encoding mode (only base64 and zlib at the moment)
- Object:Concave Polygon
- Object:TileFlipping

## Requirements

- SFML 2.x
- zlib
