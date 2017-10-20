#include <set>
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
      if (PlayStone(move.pos)) {
        // If move succeeded, reset pass counter.
        num_passes_ = 0;
        return true;
      } else {
        return false;
      }
    }
  }
  return false;
}

bool Game::PlayStone(const Pos& pos) {
  if (!IsOnBoard(pos)) {
    // Outside of the board.
    return false;
  }
  if (board_.GetPos(pos) != Color::kNone) {
    // Illegal to play on top of another stone.
    return false;
  }

  // 
  num_passes_ = 0;

  // TODO: Should we be mutating the board directly?
  board_.SetPos(pos, curr_player_);

  for (Pos neighbor : GetNeighbors(pos)) {
    // See if any neighboring groups were captured.
    TryLift(neighbor);
  }

  // See if this was a self-capture (we allow self-capture).
  TryLift(pos);

  // TODO: Check for KO?

  ++stones_played_;
  return true;
}

bool Game::IsOnBoard(const Pos& pos) const {
  if (pos.x < 0 || pos.x >= board_.width() ||
      pos.y < 0 || pos.y >= board_.height()) {
    return false;
  } else {
    return true;
  }
}

// Get all neighbors to pos that are on the board.
std::vector<Pos> Game::GetNeighbors(const Pos& pos) const {
  // All possible neighbors.
  std::vector<Pos> poss_neighs;
  poss_neighs.emplace_back(pos.x - 1, pos.y);
  poss_neighs.emplace_back(pos.x + 1, pos.y);
  poss_neighs.emplace_back(pos.x, pos.y - 1);
  poss_neighs.emplace_back(pos.x, pos.y + 1);
  std::vector<Pos> neighbors;
  for (const Pos& poss_neigh : poss_neighs) {
    if (IsOnBoard(poss_neigh)) {
      neighbors.push_back(poss_neigh);
    }
  }
  return neighbors;      
}

// Pop an element from std::set.
template<typename T, typename Cmp>
T Pop(std::set<T, Cmp>* ts) {
  auto it = ts->begin();
  T ret = *it;
  ts->erase(it);
  return ret;
}

struct PosComp {
  bool operator() (const Pos& a, const Pos& b) const {
    return std::pair<int, int>(a.x, a.y) < std::pair<int, int>(b.x, b.y);
  }
};

typedef std::set<Pos, PosComp> PosSet;

// Check to see if string of stones connected to pos should be removed from
// the board and remove them if so.
void Game::TryLift(const Pos& init_pos) {
  Color init_color = board_.GetPos(init_pos);
  if (init_color == Color::kNone) {
    // Nothing to remove.
    return;
  }

  // Flood fill algorithm.
  PosSet to_visit;
  to_visit.insert(init_pos);
  PosSet visited;
  PosSet this_string;
  while (!to_visit.empty()) {
    Pos pos = Pop(&to_visit);
    if (visited.find(pos) != visited.end()) {
      // Skip positions we've already considered.
      continue;
    }
    visited.insert(pos);
    Color this_color = board_.GetPos(pos);
    if (this_color == Color::kNone) {
      // There is at least one liberty.
      return;
    }

    if (this_color == init_color) {
      this_string.insert(pos);
      for (Pos neighbor : GetNeighbors(pos)) {
        to_visit.insert(neighbor);
      }
    }
  }

  // If we reached this point, string has no liberties. Remove it.
  for (Pos pos : this_string) {
    board_.SetPos(pos, Color::kNone);
  }
}

void Game::ScoreBoard() {
  // TODO

  // TODO
  score_ = 0;
  winner_ = Color::kBlack;
}

}  // namespace go_zero
