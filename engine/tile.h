#pragma once

#include <SFML/Graphics.hpp>

enum class TileID : uint64_t;

namespace ng {

class Tile {
 public:
  explicit Tile(TileID id);
  Tile(TileID id, sf::IntRect texture_coords);

  [[nodiscard]] TileID GetID() const;
  [[nodiscard]] const std::optional<sf::IntRect>& GetTextureCoords() const;

 private:
  TileID id_{};
  std::optional<sf::IntRect> texture_coords_;
};

}  // namespace ng
