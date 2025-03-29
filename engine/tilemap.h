#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "node.h"
#include "tile.h"
#include "tileset.h"

namespace ng {

// Represents a tilemap for rendering and managing tile-based game worlds.
// It stores the tile data in an uniform grid, and it handles rendering the map
// using a vertex array for efficient drawing.
class Tilemap : public Node {
 public:
  // Constructs a Tilemap specifying the size (in tiles) of the tilemap and the
  // associated tileset.
  Tilemap(sf::Vector2u size, Tileset tileset);

  // Returns the size of the tilemap in tiles.
  sf::Vector2u GetSize() const;
  // Returns the size of each tile in the tilemap.
  sf::Vector2u GetTileSize() const;

  // Checks if the given tile position is within the bounds of the tilemap.
  bool IsWithinBounds(sf::Vector2u position) const;
  // Returns the tile at the given tile position.
  const Tile& GetTile(sf::Vector2u position) const;
  // Sets the tile at the given tile position.
  void SetTile(sf::Vector2u position, TileID tile_id);

  // Checks if the given world position is within the bounds of the tilemap.
  bool IsWithinWorldBounds(sf::Vector2f world_position) const;
  // Returns the tile at the given world position.
  const Tile& GetWorldTile(sf::Vector2f world_position) const;
  // Sets the tile at the given world position.
  void SetWorldTile(sf::Vector2f world_position, TileID tile_id);

  // Converts world coordinates to tile coordinates.
  sf::Vector2u WorldToTileSpace(sf::Vector2f world_position) const;

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  // The size of the tilemap in tiles.
  sf::Vector2u size_;
  // The tileset used by the tilemap.
  Tileset tileset_;
  // Stores tile IDs in the tilemap grid.
  std::vector<TileID> tiles_;
  // Vertex array for rendering the tilemap.
  sf::VertexArray vertices_;
};

}  // namespace ng