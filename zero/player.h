#include <vector>

class Random;

namespace go_zero {

class Features;
class Move;
class PlayerEvaluation;
class Pos;

class Player {
 public:
  virtual ~Player() {}
  virtual void Evaluate(const Features& features,
                        PlayerEvaluation* result) const = 0;
};

class PlayerEvaluation {
 public:
  double GetPolicy(int x, int y) const { return policy_[x][y]; }
  double value() const { return value_; }

  Move RandomMove(Random* random) const;

  // Used by Player to setup.
  void SetWidth(int width) { width_ = width; }
  void SetHeight(int height) { height_ = height; }
  std::vector<std::vector<double>>* MutablePolicy() { return &policy_; }
  void SetValue(double value) { value_ = value; }

 private:
  int width_ = 0;
  int height_ = 0;
  std::vector<std::vector<double>> policy_;
  double value_ = 0.0;
};

// UniformPlayer ignores input and chooses policy that weights all moves
// equally likely.
class UniformPlayer : public Player {
 public:
  void Evaluate(const Features& features,
                PlayerEvaluation* result) const final;
};

}  // namespace go_zero
