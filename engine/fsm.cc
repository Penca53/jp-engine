#include "fsm.h"
#include "state.h"
#include "transition.h"

namespace ng {

FSM::FSM(std::unique_ptr<State> entry_state) {
  current_state_ = entry_state.get();
  AddState(std::move(entry_state));
}

void FSM::AddState(std::unique_ptr<State> state) {
  states_.insert({state->GetID(), std::move(state)});
}

void FSM::AddTransition(Transition t) {
  if (!transitions_.count(t.GetFrom())) {
    transitions_.insert({t.GetFrom(), {}});
  }
  transitions_.at(t.GetFrom()).push_back(std::move(t));
}

void FSM::Update() {
  if (transitions_.count(current_state_->GetID())) {
    for (auto& t : transitions_.at(current_state_->GetID())) {
      if (t.MeetsCondition()) {
        Transit(*states_.at(t.GetTo()));
        break;
      }
    }
  }

  current_state_->Update();
}

void FSM::Transit(State& to_state) {
  current_state_->OnExit();
  current_state_ = &to_state;
  current_state_->OnEnter();
}

}  // namespace ng