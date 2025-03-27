#pragma once

#include <functional>
#include <string>

#include "state.h"

namespace ng {

class Transition {
 public:
  Transition(State::ID from, State::ID to, std::function<bool()> condition);
  bool MeetsCondition();
  const State::ID& GetFrom() const;
  const State::ID& GetTo() const;

 private:
  State::ID from_;
  State::ID to_;
  std::function<bool()> condition_;
};

}  // namespace ng