#ifndef GO_ZERO_PLAYER_H
#define GO_ZERO_PLAYER_H

#include <memory>
#include <vector>

#include "zero/grid.h"

class Random;

namespace go_zero {

class Features;
class Move;
class PlayerEvaluation;
class Pos;

class Player {
 public:
  virtual ~Player() {}
  virtual std::unique_ptr<PlayerEvaluation> Evaluate(const Features& features)
    const = 0;
};

class PlayerEvaluation {
 public:
  PlayerEvaluation(int width, int height)
    : width_(width), height_(height), policy_(width, height, 0.0) {}

  Move RandomMove(Random* random) const;

  double value() const { return value_; }
  void set_value(double value) { value_ = value; }

  const Grid<double>& policy() const { return policy_; }
  Grid<double>* mutable_policy() { return &policy_; }

 private:
  int width_ = 0;
  int height_ = 0;
  Grid<double> policy_;
  double value_ = 0.0;
};

// UniformPlayer ignores input and chooses policy that weights all moves
// equally likely.
class UniformPlayer : public Player {
 public:
  std::unique_ptr<PlayerEvaluation> Evaluate(const Features& features)
    const final;
};

}  // namespace go_zero

#endif  // GO_ZERO_PLAYER_H
