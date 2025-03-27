#pragma once

#include <string>

namespace ng {

class State {
  friend class FSM;

 public:
  using ID = std::string;

  State(ID id);

  virtual ~State() = default;

  const ID& GetID() const;

 protected:
  virtual void OnEnter();

  virtual void Update();

  virtual void OnExit();

 private:
  ID id_;
};

}  // namespace ng