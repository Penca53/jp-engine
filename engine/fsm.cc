#include "fsm.h"

#include "state.h"
#include "transition.h"

#include <memory>
#include <utility>

namespace ng {

FSM::FSM(std::unique_ptr<ng::State> entry_state)
    : current_state_(entry_state.get()) {
  AddState(std::move(entry_state));
}

void FSM::AddState(std::unique_ptr<ng::State> state) {
  states_.insert({state->GetID(), std::move(state)});
}

void FSM::AddTransition(Transition transition) {
  if (!transitions_.contains(transition.GetFrom())) {
    transitions_.insert({transition.GetFrom(), {}});
  }
  transitions_.at(transition.GetFrom()).push_back(std::move(transition));
}

void FSM::Update() {
  if (transitions_.contains(current_state_->GetID())) {
    for (auto& transition : transitions_.at(current_state_->GetID())) {
      if (transition.MeetsCondition()) {
        Transit(*states_.at(transition.GetTo()));
        break;
      }
    }
  }

  current_state_->Update();
}

void FSM::Transit(ng::State& to_state) {
  current_state_->OnExit();
  current_state_ = &to_state;
  current_state_->OnEnter();
}

}  // namespace ng