#include <memory>

#include "zero/features.h"
#include "zero/game.h"
#include "zero/player.h"

class Random;

namespace go_zero {

// Runs single rollout and returns whether or not current player won.
bool MCRollout(const Game& starting_position, const Player& player,
               int max_moves, Random* random) {
  Game game(starting_position);
  while (!game.IsGameOver() && game.moves_played() < max_moves) {
    // Get input board features (color of stone at every point, etc.)
    Features features;
    game.GetFeatures(&features);

    // Evaluate this in the Neural Network.
    std::unique_ptr<PlayerEvaluation> policy = player.Evaluate(features);

    // Keep trying to play moves until you play a legal one.
    // TODO: Will this cause problems? Maybe Pass if illegal move is chosen?
    bool successfully_moved = false;
    while (!successfully_moved) {
      // Randomly choose a move based on the policy.
      Move move = policy->RandomMove(random);
      successfully_moved = game.Play(move);
    }
  }

  return (game.winner() == starting_position.curr_player());
}

double MCPercent(const Game& starting_position, const Player& player,
                 int max_moves, Random* random, int num_runs) {
  int successes = 0;
  for (int run_num = 0; run_num < num_runs; ++run_num) {
    if (MCRollout(starting_position, player, max_moves, random)) {
      successes += 1;
    }
  }
  return static_cast<double>(successes) / num_runs;
}

}  // namespace go_zero
