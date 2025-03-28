#pragma once

#include <string>

namespace ng {

class State {
  friend class FSM;

 public:
  using ID = std::string;

  explicit State(ID id);

  State(const State& other) = default;
  State& operator=(const State& other) = default;
  State(State&& other) = default;
  State& operator=(State&& other) = default;

  virtual ~State() = default;

  [[nodiscard]] const ID& GetID() const;

 protected:
  virtual void OnEnter();

  virtual void Update();

  virtual void OnExit();

 private:
  ID id_;
};

}  // namespace ng