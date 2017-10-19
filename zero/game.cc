#include <vector>

#include "zero/game.h"

namespace go_zero {

bool Game::Play(const Move& move) {
  if (TryPlay(move)) {
    // Switch players if move worked.
    curr_player_ = OppositeColor(curr_player_);
    return true;
  }
  return false;
}

bool Game::TryPlay(const Move& move) {
  if (IsGameOver()) {
    return false;
  }

  switch (move.type) {
    case Move::kResign: {
      winner_ = OppositeColor(curr_player_);
      return true;
    }
    case Move::kPass: {
      ++num_passes_;
      if (num_passes_ >= 2) {
        ScoreBoard();
      }
      return true;
    }
    case Move::kPlayStone: {
      break;  // Continued below.
    }
  }

  if (!IsOnBoard(move)) {
    // Outside of the board.
    return false;
  }
  if (board_.GetPos(move.x, move.y) != Color::kNone) {
    // Illegal to play on top of another stone.
    return false;
  }

  // TODO: Should we be mutating the board directly?
  board_.SetPos(move.x, move.y, curr_player_);

  // See if this was a self-capture (we allow self-capture).
  TryLift(move);

  for (Move neighbor : GetNeighbors(move)) {
    // See if any neighboring groups were captured.
    TryLift(neighbor);
  }

  // TODO: Check for KO?

  ++stones_played_;
  return true;
}

bool Game::IsOnBoard(const Move& pos) const {
  if (pos.x < 0 || pos.x >= board_.width() ||
      pos.y < 0 || pos.y >= board_.height()) {
    return false;
  } else {
    return true;
  }
}

std::vector<Move> Game::GetNeighbors(const Move& pos) const {
  // All possible neighbors.
  std::vector<Move> poss_neighs;
  poss_neighs.emplace_back(Move::kPlayStone, pos.x - 1, pos.y);
  poss_neighs.emplace_back(Move::kPlayStone, pos.x + 1, pos.y);
  poss_neighs.emplace_back(Move::kPlayStone, pos.x, pos.y - 1);
  poss_neighs.emplace_back(Move::kPlayStone, pos.x, pos.y + 1);
  std::vector<Move> neighbors;
  for (const Move& poss_neigh : poss_neighs) {
    if (IsOnBoard(poss_neigh)) {
      neighbors.push_back(poss_neigh);
    }
  }
  return neighbors;      
}

void Game::TryLift(const Move& pos) {
  // TODO
}

void Game::ScoreBoard() {
  // TODO
}

}  // namespace go_zero
