#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "state.h"
#include "transition.h"

namespace ng {

// Represents a Finite State Machine (FSM) for managing game states and
// transitions.
template <typename TContext>
class FSM {
 public:
  // Creates an FSM with a context and an entry state.
  FSM(TContext* context, std::unique_ptr<State<TContext>> entry_state)
      : context_(context), current_state_(entry_state.get()) {
    AddState(std::move(entry_state));
  }

  // Returns the associated context.
  TContext* GetContext() { return context_; }

  // Adds a state to the FSM.
  void AddState(std::unique_ptr<State<TContext>> state) {
    state->SetContext(context_);
    states_.insert({state->GetID(), std::move(state)});
  }

  // Adds a transition to the FSM.
  void AddTransition(Transition<TContext> transition) {
    if (!transitions_.contains(transition.GetFrom())) {
      transitions_.insert({transition.GetFrom(), {}});
    }
    transitions_.at(transition.GetFrom()).push_back(std::move(transition));
  }

  // Updates the FSM, evaluating transitions and changing states if necessary.
  void Update() {
    if (transitions_.contains(current_state_->GetID())) {
      for (auto& transition : transitions_.at(current_state_->GetID())) {
        if (transition.MeetsCondition(*context_)) {
          Transit(states_.at(transition.GetTo()).get());
          break;
        }
      }
    }

    current_state_->Update();
  }

 private:
  void Transit(State<TContext>* to_state) {
    current_state_->OnExit();
    current_state_ = to_state;
    current_state_->OnEnter();
  }

  // The context of the FSM.
  // All the transitions and states will only be able to act and react
  // on variables stored in the context.
  TContext* context_ = nullptr;

  // Map of state IDs to states.
  std::unordered_map<std::string, std::unique_ptr<State<TContext>>> states_;
  // Map of state IDs to their associated transitions.
  std::unordered_map<std::string, std::vector<Transition<TContext>>>
      transitions_;
  // Current active state. After the construction of a FSM, the current state
  // is never null.
  State<TContext>* current_state_ = nullptr;
};

}  // namespace ng