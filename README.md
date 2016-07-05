# TiledMapXtractor (TMX)

Library for [Tiled](http://www.mapeditor.org/) maps with [SFML](https://github.com/SFML/SFML)

## Features

- Loading
- Saving
- Modification
- Orthogonal, Isometric, Staggered and Hexagonal (both with support for Axis and Index)
- Objects (not concave polygons and flipped tiles)
- All the encoding and compression formats
- External tileset (.tsx)
- Almost all .tmx data (Please use the issue tracker if your output isn't the same as your Tiled editor)

## What is not supported / TODO

As the library is young, most features will be added soon

- Multiple tilesets for rendering (only one tileset per layer)
- Tileset:Tile:ObjectGroup
- Tileset:Tile:Image
- Image:Data
- Object:Concave Polygon
- Object:TileFlipping

## Requirements

- [SFML 2.x](https://github.com/SFML/SFML)
- [pugixml](https://github.com/zeux/pugixml) (included)
- [zlib](http://www.zlib.net/)
