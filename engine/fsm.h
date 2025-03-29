#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "state.h"
#include "transition.h"

namespace ng {

// Represents a Finite State Machine (FSM) for managing game states and
// transitions.
class FSM {
 public:
  // Creates an FSM with an entry state.
  explicit FSM(std::unique_ptr<State> entry_state);

  // Adds a state to the FSM.
  void AddState(std::unique_ptr<State> state);
  // Adds a transition to the FSM.
  void AddTransition(Transition transition);
  // Updates the FSM, evaluating transitions and changing states if necessary.
  void Update();

 private:
  void Transit(State& to_state);

  // Map of state IDs to states.
  std::unordered_map<State::ID, std::unique_ptr<State>> states_;
  // Map of state IDs to their associated transitions.
  std::unordered_map<State::ID, std::vector<Transition>> transitions_;
  // Current active state. After the construction of a FSM, the current state
  // is never null.
  State* current_state_ = nullptr;
};

}  // namespace ng