#pragma once

#include <SFML/Graphics.hpp>

enum class TileID : uint64_t;

namespace ng {

class Tile {
 public:
  Tile(TileID id);
  Tile(TileID id, sf::IntRect texture_coords);

  TileID GetID() const;
  const std::optional<sf::IntRect>& GetTextureCoords() const;

 private:
  TileID id_{};
  std::optional<sf::IntRect> texture_coords_;
};

}  // namespace ng
