#include <set>
#include <vector>

#include "zero/board.h"

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

void Board::FindReachable(const Pos& init_pos, Color init_color, PosSet* string,
                          std::set<Color>* colors_reachable) const {
  if (init_color != GetPos(init_pos)) {
    // Wrong color, nothing to do.
    return;
  }

  // Flood fill algorithm.
  PosSet to_visit;
  to_visit.insert(init_pos);
  PosSet visited;
  while (!to_visit.empty()) {
    Pos pos = Pop(&to_visit);
    if (visited.find(pos) != visited.end()) {
      // Skip positions we've already considered.
      continue;
    }
    visited.insert(pos);
    Color this_color = GetPos(pos);
    if (this_color == init_color) {
      string->insert(pos);
      for (Pos neighbor : GetNeighbors(pos)) {
        to_visit.insert(neighbor);
      }
    } else {
      // TODO: Add early exists for reaching kNone or both kBlack and kWhite?
      colors_reachable->insert(this_color);
    }
  }
}

// Check to see if string of stones connected to pos should be removed from
// the board and remove them if so.
void Board::TryLift(const Pos& init_pos, Color init_color) {
  PosSet string;
  std::set<Color> colors_reachable;
  FindReachable(init_pos, init_color, &string, &colors_reachable);
  if (colors_reachable.find(Color::kNone) == colors_reachable.end()) {
    // Cannot reach kNone from init_pos (using init_color).
    // String has no liberties. Remove it.
    for (Pos pos : string) {
      SetPos(pos, Color::kNone);
    }
  }
}

// Is this empty Pos anyone's territory according to Tromp-Taylor?
// string is populated with all empty Pos considered (to avoid re-computing).
// If territory, record whose (color).
bool Board::IsTerritory(const Pos& pos, PosSet* string, Color* color) const {
  std::set<Color> colors_reachable;
  FindReachable(pos, Color::kNone, string, &colors_reachable);
  if (colors_reachable.size() == 1) {
    // Only one of the two player's color was reachable from this empty spot.
    *color = *colors_reachable.begin();
    return true;
  } else {
    return false;
  }
}

int Board::Score() const {
  int score = 0;
  // Set of Pos visited out of order (so we don't re-compute).
  PosSet visited;
  for (int x = 0; x < width_; ++x) {
    for (int y = 0; y < height_; ++y) {
      Pos pos(x, y);
      if (visited.find(pos) != visited.end()) {
        // We already scored this pos.
        continue;
      }
      switch (GetPos(pos)) {
        case Color::kBlack: {
          score += 1;
          break;
        }
        case Color::kWhite: {
          score -= 1;
          break;
        }
        case Color::kNone: {
          PosSet terr_poss;
          Color color;
          // Is this anyone's territory?
          if (IsTerritory(pos, &terr_poss, &color)) {
            // Score all of terr_poss.
            if (color == Color::kBlack) {
              score += terr_poss.size();
            } else {
              score -= terr_poss.size();
            }
          }
          // Then make sure we don't re-compute region.
          visited.insert(terr_poss.begin(), terr_poss.end());
          break;
        }
      }
    }
  }
  return score;
}  

}  // namespace go_zero
