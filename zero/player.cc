#include "zero/player.h"

#include <memory>
#include <vector>

#include "util/random.h"
#include "zero/features.h"
#include "zero/game.h"

namespace go_zero {

Move PlayerEvaluation::RandomMove(Random* random) const {
  double rand = random->UniformReal(0, 1);
  // TODO: Is this good? Unbiased? Seems like it might bias towards lower values.
  for (int x = 0; x < width_; ++x) {
    for (int y = 0; y < height_; ++y) {
      double this_prob = policy_.Get(x, y);
      if (rand < this_prob) {
        return Move(Move::Type::kPlayStone, Pos(x, y));
      }
      rand -= this_prob;
    }
  }
  return Move(Move::Type::kPass);
}


std::unique_ptr<PlayerEvaluation> UniformPlayer::Evaluate(
    const Features& features) const {
  const int width = features.width;
  const int height = features.height;
  // Probability assigned to each move (all the same).
  const double prob_each = 1.0 / (width * height);

  std::unique_ptr<PlayerEvaluation> result(new PlayerEvaluation(width, height));
  result->set_value(0.0);  // Always assumed to be equally good for both players.
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      result->mutable_policy()->Set(x, y, prob_each);
    }
  }
  return result;
}

}  // namespace go_zero
