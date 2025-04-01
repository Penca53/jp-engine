#include "follow_player.h"

#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include "engine/app.h"
#include "player.h"

namespace game {

FollowPlayer::FollowPlayer(const Player& player) : player_(&player) {}

void FollowPlayer::Start() {
  Follow();
}

void FollowPlayer::Update() {
  Follow();
}

void FollowPlayer::Follow() {
  if (!ng::App::GetInstance().IsValid(player_)) {
    return;
  }

  sf::Vector2f player_pos = player_->GetGlobalTransform().getPosition();
  sf::Vector2f new_pos(
      std::min(std::max(player_pos.x, 32.F * (32 - 19)), 32.F * (64 - 13)),
      std::max(std::min(player_pos.y, 32.F * (32 - 10)), 32.F * (10)));
  GetParent()->SetLocalPosition(new_pos);
}

}  // namespace game