#pragma once

#include "engine/node.h"
#include "player.h"

namespace game {

class FollowPlayer : public ng::Node {
 public:
  explicit FollowPlayer(const Player& player);

 protected:
  void Start() override;
  void Update() override;

 private:
  void Follow();

  const Player* player_ = nullptr;
};

}  // namespace game