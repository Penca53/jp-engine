#pragma once

#include <unordered_map>
#include "tile.h"

namespace ng {

class Tileset {
 public:
  Tileset(sf::Vector2u tile_size, const sf::Texture& texture);

  const Tile& GetTile(TileID id) const;
  void AddTile(Tile tile);
  sf::Vector2u GetTileSize() const;
  const sf::Texture& GetTexture() const;

 private:
  sf::Vector2u tile_size_;
  const sf::Texture* texture_ = nullptr;
  std::unordered_map<TileID, Tile> tiles_;
};

}  // namespace ng