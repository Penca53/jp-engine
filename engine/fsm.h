#pragma once

#include <string>
#include <unordered_map>

#include "state.h"
#include "transition.h"

namespace ng {

class FSM {
 public:
  FSM(std::unique_ptr<ng::State> entry_state);
  void AddState(std::unique_ptr<ng::State> state);
  void AddTransition(Transition transition);
  void Update();

 private:
  void Transit(ng::State& to_state);

 private:
  std::unordered_map<ng::State::ID, std::unique_ptr<ng::State>> states_;
  std::unordered_map<ng::State::ID, std::vector<Transition>> transitions_;
  ng::State* current_state_ = nullptr;
};

}  // namespace ng