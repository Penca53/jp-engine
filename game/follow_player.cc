#include "follow_player.h"

#include "engine/app.h"
#include "mario.h"

namespace game {

FollowPlayer::FollowPlayer(const Mario& mario) : mario_(&mario) {}

void FollowPlayer::Start() {
  Follow();
}

void FollowPlayer::Update() {
  Follow();
}

void FollowPlayer::Follow() {
  if (!ng::App::GetInstance().IsValid(mario_)) {
    return;
  }

  sf::Vector2f mario_pos = mario_->GetGlobalTransform().getPosition();
  sf::Vector2f new_pos(
      std::min(std::max(mario_pos.x, 32.f * (32 - 19)), 32.f * (64 - 13)),
      std::max(std::min(mario_pos.y, 32.f * (32 - 10)), 32.f * (10)));
  GetParent()->SetLocalPosition(new_pos);
}

}  // namespace game