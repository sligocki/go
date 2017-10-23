#include "zero/player.h"

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
      double this_prob = policy_[x][y];
      if (rand < this_prob) {
        return Move(Move::Type::kPlayStone, Pos(x, y));
      }
      rand -= this_prob;
    }
  }
  return Move(Move::Type::kPass);
}


void UniformPlayer::Evaluate(const Features& features,
                             PlayerEvaluation* result) const {
  const int width = features.width;
  const int height = features.height;
  // Probability assigned to each move (all the same).
  const double prob_each = 1.0 / (width * height);

  result->SetValue(0.0);  // Always assumed to be equally good for both players.
  result->SetWidth(width);
  result->SetHeight(height);
  auto* policy = result->MutablePolicy();
  for (int col = 0; col < width; ++col) {
    policy->emplace_back(height, prob_each);
  }
}

}  // namespace go_zero
