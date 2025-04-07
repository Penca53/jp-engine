#pragma once

#include "engine/node.h"
#include "player.h"

namespace game {

class FollowPlayer : public ng::Node {
 public:
  FollowPlayer(ng::App& app, const Player& player);

 protected:
  void OnAdd() override;
  void Update() override;

 private:
  void Follow();

  const Player* player_ = nullptr;
};

}  // namespace game