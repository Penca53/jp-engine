#include "background.h"
#include "engine/resource_manager.h"

namespace game {

static constexpr size_t kTriangleVertexCount = 3;
static constexpr size_t kTrisInQuad = 2 * kTriangleVertexCount;

Background::Background(sf::Vector2u size)
    : size_(size),
      texture_(&ng::ResourceManager::GetInstance().LoadTexture("Gray.png")),
      image_vertices_(sf::PrimitiveType::Triangles, kTrisInQuad) {
  texture_->setRepeated(true);

  image_vertices_[0].position = sf::Vector2f(0, 0);
  image_vertices_[1].position = sf::Vector2f(size_.x, 0);
  image_vertices_[2].position = sf::Vector2f(0, size_.y);
  image_vertices_[3].position = sf::Vector2f(0, size_.y);
  image_vertices_[4].position = sf::Vector2f(size_.x, 0);
  image_vertices_[5].position = sf::Vector2f(size_.x, size_.y);
}

void Background::Update() {
  static constexpr int32_t kScrollTicksPerPixel = 4;
  float offset = static_cast<float>(-t_) / kScrollTicksPerPixel;

  image_vertices_[0].texCoords = sf::Vector2f(0, 0 + offset);
  image_vertices_[1].texCoords = sf::Vector2f(size_.x, 0 + offset);
  image_vertices_[2].texCoords = sf::Vector2f(0, size_.y + offset);
  image_vertices_[3].texCoords = sf::Vector2f(0, size_.y + offset);
  image_vertices_[4].texCoords = sf::Vector2f(size_.x, 0 + offset);
  image_vertices_[5].texCoords = sf::Vector2f(size_.x, size_.y + offset);

  t_ = (t_ + 1) % (texture_->getSize().y * kScrollTicksPerPixel);
}

void Background::Draw(sf::RenderTarget& target) {
  sf::RenderStates state;
  state.texture = texture_;
  state.transform = GetGlobalTransform().getTransform();
  target.draw(image_vertices_, state);
}

}  // namespace game