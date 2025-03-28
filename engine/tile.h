#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <optional>

// Represents a unique identifier for a tile.
// This MUST be defined by the user of the engine.
enum class TileID : uint64_t;

namespace ng {

// Contains the data of a single tile, including its unique ID and
// optional texture coordinates within a texture sheet.
class Tile {
 public:
  // Creates a Tile with the given ID and no texture coordinates.
  // This is useful for "empty" tiles that need no rendering.
  explicit Tile(TileID id);
  // Creates a Tile with the given ID and the specified texture coordinates.
  Tile(TileID id, sf::IntRect texture_coords);

  // Returns the ID of the tile.
  [[nodiscard]] TileID GetID() const;
  // Returns the texture coordinates of the tile, if any.
  [[nodiscard]] const std::optional<sf::IntRect>& GetTextureCoords() const;

 private:
  // The unique ID of the tile.
  TileID id_{};
  // Optional texture coordinates of the tile.
  // It is optional because invisible or empty tiles don't render anything.
  std::optional<sf::IntRect> texture_coords_;
};

}  // namespace ng
