#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "state.h"
#include "transition.h"

namespace ng {

class FSM {
 public:
  FSM(std::unique_ptr<State> entry_state);
  void AddState(std::unique_ptr<State> state);
  void AddTransition(Transition transition);
  void Update();

 private:
  void Transit(State& to_state);

 private:
  std::unordered_map<State::ID, std::unique_ptr<State>> states_;
  std::unordered_map<State::ID, std::vector<Transition>> transitions_;
  State* current_state_ = nullptr;
};

}  // namespace ng