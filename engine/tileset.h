#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>

#include "tile.h"

namespace ng {

// Represents a collection of tiles, managed from a texture sheet, and accessed
// by tile IDs.
class Tileset {
 public:
  // Creates a tileset given the size of a tile in pixels,
  // and the texture sheet to map tiles from.
  Tileset(sf::Vector2u tile_size, const sf::Texture* texture);

  // Returns the tile, given its ID.
  [[nodiscard]] const Tile& GetTile(TileID id) const;
  // Adds a tile to the tileset.
  void AddTile(Tile tile);

  // Returns the size of a tile in the tileset.
  [[nodiscard]] sf::Vector2u GetTileSize() const;
  // Returns the texture sheet.
  [[nodiscard]] const sf::Texture* GetTexture() const;

 private:
  // The size of every tile in the tileset.
  sf::Vector2u tile_size_;
  // The texture sheet.
  const sf::Texture* texture_ = nullptr;
  // The actual mapping of tiles to tile IDs.
  std::unordered_map<TileID, Tile> tiles_;
};

}  // namespace ng