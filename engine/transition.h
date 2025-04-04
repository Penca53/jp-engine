#pragma once

#include <functional>
#include <string>

#include "state.h"

namespace ng {

// Represents a transition between two states in a state machine, governed by
// a condition.
class Transition {
 public:
  // Creates a transition given the source and destination state IDs, and a
  // condition function.
  Transition(ng::State::ID from, ng::State::ID to,
             std::function<bool()> condition);

  // Returns the source state ID.
  [[nodiscard]] const ng::State::ID& GetFrom() const;
  // Returns the destination state ID.
  [[nodiscard]] const ng::State::ID& GetTo() const;

  // Invokes the condition function and returns its return value.
  [[nodiscard]] bool MeetsCondition() const;

 private:
  // The source state of the transition.
  ng::State::ID from_;
  // The destination state of the transition.
  ng::State::ID to_;
  // The condition bound to the transition.
  std::function<bool()> condition_;
};

}  // namespace ng