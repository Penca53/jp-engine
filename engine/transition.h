#pragma once

#include <functional>
#include <string>

#include "state.h"

namespace ng {

class Transition {
 public:
  Transition(ng::State::ID from, ng::State::ID to,
             std::function<bool()> condition);
  bool MeetsCondition();
  const ng::State::ID& GetFrom() const;
  const ng::State::ID& GetTo() const;

 private:
  ng::State::ID from_;
  ng::State::ID to_;
  std::function<bool()> condition_;
};

}  // namespace ng