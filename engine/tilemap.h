#pragma once

#include "node.h"
#include "tile.h"
#include "tileset.h"

namespace ng {

class Tilemap : public Node {
 public:
  Tilemap(sf::Vector2u size, Tileset tileset);

  sf::Vector2u GetSize() const;
  sf::Vector2u GetTileSize() const;

  bool IsWithinBounds(sf::Vector2u position) const;
  const Tile& GetTile(sf::Vector2u position) const;
  void SetTile(sf::Vector2u position, TileID tile_id);

  bool IsWithinWorldBounds(sf::Vector2f world_position) const;
  const Tile& GetWorldTile(sf::Vector2f world_position) const;
  void SetWorldTile(sf::Vector2f world_position, TileID tile_id);

  sf::Vector2u WorldToTileSpace(sf::Vector2f world_position) const;

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  sf::Vector2u size_;
  Tileset tileset_;
  std::vector<TileID> tiles_;
  sf::VertexArray vertices_;
};

}  // namespace ng