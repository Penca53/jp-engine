#include "state.h"

#include <utility>

namespace ng {

State::State(ID id) : id_(std::move(id)) {}

const State::ID& State::GetID() const {
  return id_;
}

void State::OnEnter() {}

void State::Update() {}

void State::OnExit() {}

}  // namespace ng
