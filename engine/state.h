#pragma once

#include <string>

namespace ng {

// Represents a state in a Finite State Machine (FSM).
// The State class serves as a base class for defining individual states within
// an FSM. Each state has a unique ID, which is used to identify and manage
// transitions between states.
class State {
  // FSM needs to be able to call OnEnter, Update, and OnExit.
  friend class FSM;

 public:
  using ID = std::string;

  // Creates a state with the given unique ID.
  explicit State(ID id);

  State(const State& other) = default;
  State& operator=(const State& other) = default;
  State(State&& other) = default;
  State& operator=(State&& other) = default;

  virtual ~State() = default;

  // Returns the ID of the state.
  [[nodiscard]] const ID& GetID() const;

 protected:
  // Called when the state is entered.
  virtual void OnEnter();

  // Called during each update cycle of the state.
  virtual void Update();

  // Called when the state is exited.
  virtual void OnExit();

 private:
  // The unique ID of the state.
  ID id_;
};

}  // namespace ng