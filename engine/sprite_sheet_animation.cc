#include "sprite_sheet_animation.h"

#include <SFML/Graphics/Sprite.hpp>
#include <cstdint>
#include <functional>
#include <string>
#include <utility>

#include "app.h"

namespace ng {

SpriteSheetAnimation::SpriteSheetAnimation(sf::Sprite& sprite,
                                           const std::string& texture,
                                           int32_t ticks_per_frame)
    : sprite_(&sprite),
      texture_(&ng::App::GetInstance().GetMutableResourceManager().LoadTexture(
          texture)),
      ticks_per_frame_(ticks_per_frame),
      frame_size_({static_cast<int32_t>(texture_->getSize().y),
                   static_cast<int32_t>(texture_->getSize().y)}) {
  frames_count_ = static_cast<int32_t>(texture_->getSize().x) / frame_size_.x;
}

SpriteSheetAnimation::SpriteSheetAnimation(sf::Sprite& sprite,
                                           const std::string& texture,
                                           int32_t ticks_per_frame,
                                           sf::Vector2i frame_size)
    : sprite_(&sprite),
      texture_(&ng::App::GetInstance().GetMutableResourceManager().LoadTexture(
          texture)),
      ticks_per_frame_(ticks_per_frame),
      frame_size_(frame_size) {
  frames_count_ = static_cast<int32_t>(texture_->getSize().x) / frame_size_.x;
}

int32_t SpriteSheetAnimation::GetFrameIndex() const {
  return frame_index_;
}

int32_t SpriteSheetAnimation::GetTicksCounter() const {
  return ticks_counter_;
}

void SpriteSheetAnimation::RegisterOnEndCallback(
    std::function<void()> on_end_callback) {
  on_end_callback_ = std::move(on_end_callback);
}

void SpriteSheetAnimation::Start() {
  frame_index_ = 0;
  ticks_counter_ = 0;
  sprite_->setTexture(*texture_);
}

void SpriteSheetAnimation::Update() {
  sprite_->setTextureRect(
      sf::IntRect({frame_index_ * frame_size_.x, 0}, frame_size_));

  ++ticks_counter_;
  if (ticks_counter_ >= ticks_per_frame_) {
    ticks_counter_ = 0;
    frame_index_ = (frame_index_ + 1) % frames_count_;
    if (frame_index_ == 0) {
      if (on_end_callback_.has_value()) {
        (*on_end_callback_)();
      }
    }
  }
}

}  // namespace ng
