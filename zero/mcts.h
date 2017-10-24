#ifndef GO_ZERO_MCTS_H
#define GO_ZERO_MCTS_H

#include <memory>

#include "zero/grid.h"
#include "zero/game.h"
#include "zero/player.h"

class Random;

namespace go_zero {

struct GameTree {
  GameTree(const Game& state, std::unique_ptr<PlayerEvaluation> evaluation)
    : state(state),
    evaluation_(std::move(evaluation)),
      children_(state.width(), state.height(), nullptr) {}

  Game state;
  std::unique_ptr<PlayerEvaluation> evaluation_;
  Grid<GameTree*> children_;
};

class MCTS {
 public:
  MCTS() {}

  // Search through GameTree til we reach first undefined node and add it
  // (updating action scores).
  void SearchOnce();

  // Evaluate many searches and then make one move.
  void MoveOnce();

  // Repeatedly play moves in a game.
  void PlayGame();

 private:
  // Number of Monte Carlo Tree Searches to perform before each move.
  const int searches_per_move_ = 0;

  // Current root of MCTS game tree (root is current position).
  GameTree* root_ = nullptr;
  // Moves played from start to now.
  std::vector<Move> moves_;
};

// Legacy methods: ...

// Runs single rollout and returns whether or not current player won.
bool MCRollout(const Game& starting_position, const Player& player,
               int max_moves, Random* random);

double MCPercent(const Game& starting_position, const Player& player,
                 int max_moves, Random* random, int num_runs);

}  // namespace go_zero

#endif  // GO_ZERO_MCTS_H
