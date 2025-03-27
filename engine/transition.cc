#include "transition.h"
#include "state.h"

#include <functional>
#include <string>

namespace ng {

Transition::Transition(State::ID from, State::ID to,
                       std::function<bool()> condition)
    : from_(std::move(from)),
      to_(std::move(to)),
      condition_(std::move(condition)) {}

bool Transition::MeetsCondition() {
  return condition_();
}

const State::ID& Transition::GetFrom() const {
  return from_;
}

const State::ID& Transition::GetTo() const {
  return to_;
}

}  // namespace ng