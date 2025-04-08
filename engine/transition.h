#pragma once

#include <functional>
#include <string>

#include "state.h"

namespace ng {

// Represents a transition between two states in a state machine, governed by
// a condition.
template <typename TContext>
class Transition {
 public:
  // Creates a transition given the source and destination state IDs, and a
  // condition function.
  Transition(State<TContext>::ID from, State<TContext>::ID to,  // NOLINT
             std::function<bool(TContext&)> condition)
      : from_(std::move(from)),
        to_(std::move(to)),
        condition_(std::move(condition)) {}

  // Returns the source state ID.
  [[nodiscard]] const State<TContext>::ID& GetFrom() const { return from_; }

  // Returns the destination state ID.
  [[nodiscard]] const State<TContext>::ID& GetTo() const { return to_; }

  // Invokes the condition function and returns its return value.
  [[nodiscard]] bool MeetsCondition(TContext& context) const {
    return condition_(context);
  }

 private:
  // The source state of the transition.
  State<TContext>::ID from_;
  // The destination state of the transition.
  State<TContext>::ID to_;
  // The condition bound to the transition.
  std::function<bool(TContext&)> condition_;
};

}  // namespace ng