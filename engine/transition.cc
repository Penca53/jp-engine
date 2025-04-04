#include "transition.h"
#include "state.h"

#include <functional>
#include <utility>

namespace ng {

Transition::Transition(ng::State::ID from, ng::State::ID to,
                       std::function<bool()> condition)
    : from_(std::move(from)),
      to_(std::move(to)),
      condition_(std::move(condition)) {}

bool Transition::MeetsCondition() const {
  return condition_();
}

const ng::State::ID& Transition::GetFrom() const {
  return from_;
}

const ng::State::ID& Transition::GetTo() const {
  return to_;
}

}  // namespace ng