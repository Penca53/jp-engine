#include "tilemap.h"

#include "resource_manager.h"
#include "tile.h"

namespace ng {

namespace {

static constexpr size_t kTriangleVertexCount = 3;
static constexpr size_t kTrisInQuad = 2 * kTriangleVertexCount;

}  // namespace

Tilemap::Tilemap(sf::Vector2u size, Tileset tileset)
    : size_(size),
      tileset_(std::move(tileset)),
      vertices_(sf::PrimitiveType::Triangles, size_.x * size_.y * kTrisInQuad) {
  tiles_.resize(size_.x * size_.y);

  for (uint32_t y = 0; y < size_.y; ++y) {
    for (uint32_t x = 0; x < size_.x; ++x) {
      sf::Vertex* triangles = &vertices_[(y * size_.x + x) * kTrisInQuad];

      sf::Vector2u tile_size = tileset_.GetTileSize();

      triangles[0].position = sf::Vector2f(x * tile_size.x, y * tile_size.y);
      triangles[1].position =
          sf::Vector2f((x + 1) * tile_size.x, y * tile_size.y);
      triangles[2].position =
          sf::Vector2f(x * tile_size.x, (y + 1) * tile_size.y);
      triangles[3].position =
          sf::Vector2f(x * tile_size.x, (y + 1) * tile_size.y);
      triangles[4].position =
          sf::Vector2f((x + 1) * tile_size.x, y * tile_size.y);
      triangles[5].position =
          sf::Vector2f((x + 1) * tile_size.x, (y + 1) * tile_size.y);

      triangles[0].color = sf::Color::Transparent;
      triangles[1].color = sf::Color::Transparent;
      triangles[2].color = sf::Color::Transparent;
      triangles[3].color = sf::Color::Transparent;
      triangles[4].color = sf::Color::Transparent;
      triangles[5].color = sf::Color::Transparent;
    }
  }
}

sf::Vector2u Tilemap::GetSize() const {
  return size_;
}

sf::Vector2u Tilemap::GetTileSize() const {
  return tileset_.GetTileSize();
}

bool Tilemap::IsWithinBounds(sf::Vector2u position) const {
  return position.x < size_.x && position.y < size_.y;
}

const Tile& Tilemap::GetTile(sf::Vector2u position) const {
  return tileset_.GetTile(tiles_[position.y * size_.x + position.x]);
}

void Tilemap::SetTile(sf::Vector2u position, TileID tile_id) {
  tiles_[position.y * size_.x + position.x] = tile_id;

  sf::Vertex* triangles =
      &vertices_[(position.y * size_.x + position.x) * kTrisInQuad];

  auto texture_coords = GetTile(position).GetTextureCoords();
  if (texture_coords.has_value()) {
    triangles[0].texCoords =
        sf::Vector2f(texture_coords->position.x, texture_coords->position.y);
    triangles[1].texCoords =
        sf::Vector2f(texture_coords->position.x + texture_coords->size.x,
                     texture_coords->position.y);
    triangles[2].texCoords =
        sf::Vector2f(texture_coords->position.x,
                     texture_coords->position.y + texture_coords->size.y);
    triangles[3].texCoords =
        sf::Vector2f(texture_coords->position.x,
                     texture_coords->position.y + texture_coords->size.y);
    triangles[4].texCoords =
        sf::Vector2f(texture_coords->position.x + texture_coords->size.x,
                     texture_coords->position.y);
    triangles[5].texCoords =
        sf::Vector2f(texture_coords->position.x + texture_coords->size.x,
                     texture_coords->position.y + texture_coords->size.y);

    triangles[0].color = sf::Color::White;
    triangles[1].color = sf::Color::White;
    triangles[2].color = sf::Color::White;
    triangles[3].color = sf::Color::White;
    triangles[4].color = sf::Color::White;
    triangles[5].color = sf::Color::White;
  } else {
    triangles[0].color = sf::Color::Transparent;
    triangles[1].color = sf::Color::Transparent;
    triangles[2].color = sf::Color::Transparent;
    triangles[3].color = sf::Color::Transparent;
    triangles[4].color = sf::Color::Transparent;
    triangles[5].color = sf::Color::Transparent;
  }
}

bool Tilemap::IsWithinWorldBounds(sf::Vector2f world_position) const {
  if (world_position.x < 0 || world_position.y < 0) {
    return false;
  }

  return IsWithinBounds(WorldToTileSpace(world_position));
}

const Tile& Tilemap::GetWorldTile(sf::Vector2f world_position) const {
  return GetTile(WorldToTileSpace(world_position));
}

void Tilemap::SetWorldTile(sf::Vector2f world_position, TileID tile_id) {
  SetTile(WorldToTileSpace(world_position), tile_id);
}

sf::Vector2u Tilemap::WorldToTileSpace(sf::Vector2f world_position) const {
  return sf::Vector2u(
      (world_position - GetGlobalTransform().getPosition())
          .componentWiseDiv(sf::Vector2f(tileset_.GetTileSize())));
}

void Tilemap::Draw(sf::RenderTarget& target) {
  sf::RenderStates state;
  state.transform = GetGlobalTransform().getTransform();
  state.texture = &tileset_.GetTexture();
  target.draw(vertices_, state);
}

}  // namespace ng