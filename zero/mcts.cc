#include "zero/mcts.h"

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>

#include "zero/board_io.h"
#include "zero/features.h"
#include "zero/game.h"
#include "zero/grid.h"
#include "zero/player.h"

class Random;

namespace go_zero {

GameTree::GameTree(double decay, const Game& state, const Player& player,
                   const GameTree* parent)
  : decay_(decay),
    state_(state),
    children_(state.width(), state.height()),
    visits_(state.width(), state.height()),
    action_(state.width(), state.height()),
    parent_(parent) {
  // Evaluate position
  Features features;
  state.GetFeatures(&features);
  evaluation_ = std::move(player.Evaluate(features));

  // Initialize Action weights.
  // TODO: Add Move loop iteration (as explained in https://stackoverflow.com/questions/8164567/how-to-make-my-custom-type-to-work-with-range-based-for-loops)
  for (int x = 0; x < action_.width(); ++x) {
    for (int y = 0; y < action_.height(); ++y) {
      Move move(Move::Type::kPlayStone, Pos(x, y));
      action_.Get(move) = evaluation_->policy().Get(move);
    }
  }
  Move move(Move::Type::kPass);
  // TODO: Make sure this is doing a data copy (not ref copy).
  action_.Get(move) = evaluation_->policy().Get(move);
}

Move GameTree::RandomMove(Random* random) const {
  double total_weight = 0.0;
  for (int x = 0; x < action_.width(); ++x) {
    for (int y = 0; y < action_.height(); ++y) {
      Move move(Move::Type::kPlayStone, Pos(x, y));
      total_weight += action_.Get(move);
    }
  }
  {
    Move move(Move::Type::kPass);
    total_weight += action_.Get(move);
  }

  double rand = random->UniformReal(0, total_weight);

  // TODO: Is this biased?
  for (int x = 0; x < action_.width(); ++x) {
    for (int y = 0; y < action_.height(); ++y) {
      Move move(Move::Type::kPlayStone, Pos(x, y));
      double this_weight = action_.Get(move);
      if (rand < this_weight) {
        return move;
      }
      rand -= this_weight;
    }
  }
  return Move(Move::Type::kPass);
}

void GameTree::AddVisit(Move move) {
  visits_.Get(move) += 1;
  action_.Get(move) *= decay_;
}

MCTS::MCTS(int searches_per_move, int max_moves, double decay,
           const Game& initial_state, const Player* player)
  : searches_per_move_(searches_per_move),
    max_moves_(max_moves),
    decay_(decay),
    player_(player),
    root_(decay_, initial_state, *player_, nullptr),
    current_node_(&root_) {}
  

void MCTS::SearchOnce() {
  GameTree* node = current_node_;
  while (true) {
    // Choose a random move to explore.
    Move move = node->RandomMove(&random_);
    if (node->child(move) == nullptr) {
      // Unexplored node. Evaluate it and finish.
      Game new_state(node->state());
      if (new_state.Play(move)) {
        node->set_child(move, new GameTree(decay_, new_state, *player_, node));
        // TODO: Update Actions for all parents.
      } else {
        // TODO: What to do about illegal moves? Right now we do less
        // exploration because of them.
      }
      return;
    }
    node->AddVisit(move);
    node = node->child(move);
  }
}

void MCTS::MoveOnce() {
  // Explore tree.
  for (int i = 0; i < searches_per_move_; ++i) {
    SearchOnce();
  }
  // Choose a move. We just choose the move we've explored the most (which
  // is a proxy for the best move because we bias our search towards the move
  // we think is best).
  int max_visits = -1;
  std::vector<Move> best_moves;
  // TODO: for(Move move, int visits : current_node_.visits())
  for (int x = 0; x < current_node_->width(); ++x) {
    for (int y = 0; y < current_node_->height(); ++y) {
      Move move(Move::Type::kPlayStone, Pos(x, y));
      int visits = current_node_->visits().Get(move);
      if (visits > max_visits) {
        max_visits = visits;
        best_moves.clear();
        best_moves.push_back(move);
      } else if (visits == max_visits) {
        best_moves.push_back(move);
      }
    }
  }
  {
    Move move(Move::Type::kPass);
    int visits = current_node_->visits().Get(move);
    if (visits > max_visits) {
      max_visits = visits;
      best_moves.clear();
      best_moves.push_back(move);
    } else if (visits == max_visits) {
      best_moves.push_back(move);
    }
  }
  std::cout << "max_visits = " << max_visits
            << " best_moves.size() = " << best_moves.size() << std::endl;

  // Pick a random best move.
  Move move = random_.Choose(best_moves);
  std::cout << " move = Move(type=" << (int)move.type
            << ", x=" << move.pos.x << ", y=" << move.pos.y << ")" << std::endl;

  moves_.push_back(move);
  current_node_ = current_node_->child(move);
  assert(current_node_ != nullptr);

  std::cout << BoardToString(current_node_->state().board());
}

void MCTS::SelfPlayGame() {
  while (!current_node_->state().IsGameOver() &&
         current_node_->state().moves_played() < max_moves_) {
    MoveOnce();
  }
  // TODO: Return moves_ + winner?
}

}  // namespace go_zero
