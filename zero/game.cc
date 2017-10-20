#include <set>
#include <vector>

#include "zero/game.h"

namespace go_zero {

bool Board::PlayStone(const Pos& pos, Color color) {
  if (!IsOnBoard(pos)) {
    // Outside of the board.
    return false;
  }
  if (GetPos(pos) != Color::kNone) {
    // Illegal to play on top of another stone.
    return false;
  }

  // TODO: Should we be mutating the board directly?
  SetPos(pos, color);

  Color other_color = OppositeColor(color);
  for (Pos neighbor : GetNeighbors(pos)) {
    // See if any neighboring groups were captured.
    TryLift(neighbor, other_color);
  }

  // See if this was a self-capture (we allow self-capture).
  TryLift(pos, color);

  // TODO: Check for KO?
  return true;
}

bool Board::IsOnBoard(const Pos& pos) const {
  if (pos.x < 0 || pos.x >= width_ ||
      pos.y < 0 || pos.y >= height_) {
    return false;
  } else {
    return true;
  }
}

// Get all neighbors to pos that are on the board.
std::vector<Pos> Board::GetNeighbors(const Pos& pos) const {
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
void Board::TryLift(const Pos& init_pos, Color init_color) {
  if (init_color != GetPos(init_pos)) {
    // Wrong color, nothing to do.
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
    Color this_color = GetPos(pos);
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
    SetPos(pos, Color::kNone);
  }
}

int Board::Score() const {
  // TODO
  return 0;
}  


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
        score_ = board_.Score();
        winner_ = (score_ - komi_ > 0 ? Color::kBlack : Color::kWhite);
      }
      return true;
    }
    case Move::kPlayStone: {
      if (board_.PlayStone(move.pos, curr_player_)) {
        ++stones_played_;
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

}  // namespace go_zero
