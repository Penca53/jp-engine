#pragma once

#include "engine/node.h"
#include "mario.h"

namespace game {

class FollowPlayer : public ng::Node {
 public:
  explicit FollowPlayer(const Mario& mario);

 protected:
  void Start() override;
  void Update() override;

 private:
  void Follow();

  const Mario* mario_ = nullptr;
};

}  // namespace game