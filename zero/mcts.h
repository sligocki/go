#ifndef GO_ZERO_MCTS_H
#define GO_ZERO_MCTS_H

#include <memory>

#include "util/random.h"
#include "zero/game.h"
#include "zero/player.h"

namespace go_zero {

class GameTree {
 public:
  GameTree(double decay, const Game& state, const Player& player,
           const GameTree* parent);

  // Choose a random next move to explore based on action_ weights.
  Move RandomMove(Random* random) const;

  void AddVisit(Move move);

  const Game& state() const { return state_; }
  GameTree* child(Move move) {
    return children_.Get(move).get();
  }
  void set_child(Move move, GameTree* child) {
    children_.Get(move).reset(child);
  }

  int width() const { return state_.width(); }
  int height() const { return state_.height(); }

 private:
  // Fraction to reduce action_ each time we visit a position (ex: 0.9 means
  // we will be 90% as likely to visit this node again after first visit,
  // 81% after second visit, etc.)
  const double decay_ = 0.0;
  Game state_;
  std::unique_ptr<PlayerEvaluation> evaluation_;
  MoveMap<std::unique_ptr<GameTree>> children_;
  // Number of times we've visited each child.
  MoveMap<int> visits_;
  // Action value for children.
  MoveMap<double> action_;
  const GameTree* parent_ = nullptr;
};

class MCTS {
 public:
  MCTS(int searches_per_move, int max_moves, double decay,
       const Game& initial_state, const Player* player);

  // Search through GameTree til we reach first undefined node and add it
  // (updating action scores).
  void SearchOnce();

  // Evaluate many searches and then make one move.
  void MoveOnce();

  // Repeatedly play moves in a game.
  // Note: Both players share the same memory, so this is not a very
  // competitive game.
  void SelfPlayGame();

 private:
  // Number of Monte Carlo Tree Searches to perform before each move.
  const int searches_per_move_ = 0;
  // Max moves to play in a game before declaring it over.
  const int max_moves_ = 0;
  const double decay_ = 0.0;

  const Player* player_ = nullptr;

  // Root of MCTS game tree.
  GameTree root_;
  // Current node in game tree (current game position).
  GameTree* current_node_ = nullptr;
  // Moves played from start to now.
  std::vector<Move> moves_;

  Random random_;
};

// Legacy methods: ...

// Runs single rollout and returns whether or not current player won.
bool MCRollout(const Game& starting_position, const Player& player,
               int max_moves, Random* random);

double MCPercent(const Game& starting_position, const Player& player,
                 int max_moves, Random* random, int num_runs);

}  // namespace go_zero

#endif  // GO_ZERO_MCTS_H
