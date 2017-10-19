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

  if (!OnBoard(move)) {
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
  TryLift(move.x, move.y);

  for (Move neighbor : Neighbors(move)) {
    // See if any neighboring groups were captured.
    TryLift(neighbor.x, neighbor.y);
  }

  // TODO: Check for KO?

  ++stones_played_;
  return true;
}

bool Game::OnBoard(const Move& pos) {
  if (move.x < 0 || move.x >= board_.width() ||
      move.y < 0 || move.y >= board_.height()) {
    return false;
  } else {
    return true;
  }
}

std::vector<Move> Game::GetNeighbors(const Move& pos) {
  // All possible neighbors.
  std::vector<Move> poss_neighs { { kPlayStone, pos.x - 1, pos.y },
                                  { kPlayStone, pos.x + 1, pos.y },
                                  { kPlayStone, pos.x, pos.y - 1 },
                                  { kPlayStone, pos.x, pos.y + 1 } };
  std::vector<Move> neighbors;
  for (const Move& poss_neigh : poss_neighs) {
    if (OnBoard(poss_neigh)) {
      neighbors.push_back(poss_neigh);
    }
  }
  return neighbors;      
}

void Game::TryLift(const Move& pos) {
  // TODO
}

}  // namespace go_zero
