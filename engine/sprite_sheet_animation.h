#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

namespace ng {

class SpriteSheetAnimation {
 public:
  SpriteSheetAnimation(sf::Sprite& sprite, const std::string& texture,
                       int32_t ticks_per_frame);
  SpriteSheetAnimation(sf::Sprite& sprite, const std::string& texture,
                       int32_t ticks_per_frame, sf::Vector2i frame_size);

  [[nodiscard]] int32_t GetFrameIndex() const;
  [[nodiscard]] int32_t GetTicksCounter() const;
  void RegisterOnEndCallback(std::function<void()> on_end_callback);

  void Start();
  void Update();

 private:
  sf::Sprite* sprite_ = nullptr;
  const sf::Texture* texture_ = nullptr;
  int32_t ticks_per_frame_ = 0;
  int32_t frame_index_ = 0;
  int32_t ticks_counter_ = 0;
  sf::Vector2i frame_size_;
  int32_t frames_count_ = 0;
  std::optional<std::function<void()>> on_end_callback_;
};

}  // namespace ng
