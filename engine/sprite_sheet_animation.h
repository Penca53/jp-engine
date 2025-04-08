#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>

namespace ng {

// The SpriteSheetAnimation class provides functionality for animating a sprite
// using a sprite sheet. It allows you to specify the texture, frame size, and
// animation speed. The animation automatically loops around.
// It supports registering a callback function to be executed when the
// animation finishes.
class SpriteSheetAnimation {
 public:
  // Creates a SpriteSheetAnimation.
  // This overload deduces the frame's size to be a square
  // with side equals to the height of the texture.
  SpriteSheetAnimation(sf::Sprite* sprite, const sf::Texture* texture,
                       int32_t ticks_per_frame);
  // Constructs a SpriteSheetAnimation with a specified frame size.
  // This overload is useful when a frame isn't a regular square.
  SpriteSheetAnimation(sf::Sprite* sprite, const sf::Texture* texture,
                       int32_t ticks_per_frame, sf::Vector2i frame_size);

  // Returns the current frame index.
  [[nodiscard]] int32_t GetFrameIndex() const;
  // Returns the current ticks counter.
  [[nodiscard]] int32_t GetTicksCounter() const;
  // Registers a callback function to be executed when the animation finishes.
  void RegisterOnEndCallback(std::function<void()> on_end_callback);

  // Starts the animation.
  void Start();
  // Updates the animation, advancing to the next frame if necessary.
  void Update();

 private:
  // The sprite being animated.
  sf::Sprite* sprite_ = nullptr;
  // Texture containing the sprite sheet.
  const sf::Texture* texture_ = nullptr;
  // Number of ticks per animation frame.
  // Higher ticks = slower animation speed.
  int32_t ticks_per_frame_ = 0;
  // Current animation frame index.
  int32_t frame_index_ = 0;
  // Current ticks counter.
  // Used for timing the animation frame advancement.
  int32_t ticks_counter_ = 0;
  // Size of each frame in the sprite sheet.
  sf::Vector2i frame_size_;
  // Total number of frames in the animation.
  int32_t frames_count_ = 0;
  // Optional callback function to be executed when the animation ends.
  std::optional<std::function<void()>> on_end_callback_;
};

}  // namespace ng
