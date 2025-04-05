#pragma once

#include <string>

namespace ng {

// Represents a state in a Finite State Machine (FSM).
// The State class serves as a base class for defining individual states within
// an FSM. Each state has a unique ID, which is used to identify and manage
// transitions between states.
template <typename TContext>
class State {
  // FSM needs to be able to call OnEnter, Update, OnExit and SetContext.
  template <typename>
  friend class FSM;

 public:
  using ID = std::string;

  // Creates a state with the given unique ID.
  explicit State(ID id) : id_(std::move(id)) {}

  State(const State& other) = default;
  State& operator=(const State& other) = default;
  State(State&& other) = default;
  State& operator=(State&& other) = default;

  virtual ~State() = default;

  // Returns the ID of the state.
  [[nodiscard]] const ID& GetID() const { return id_; }

 protected:
  // Returns the context associated to the state.
  // The context can be null, for example when a State is not added to a FSM.
  TContext* GetContext() { return context_; }

  // Called when the state is entered.
  virtual void OnEnter() {}

  // Called during each update cycle of the state.
  virtual void Update() {}

  // Called when the state is exited.
  virtual void OnExit() {}

 private:
  void SetContext(TContext& context) { context_ = &context; }

  // The unique ID of the state.
  ID id_;
  // The (nullable) context, usually populated by the FSM once the state is
  // added.
  TContext* context_ = nullptr;
};

}  // namespace ng