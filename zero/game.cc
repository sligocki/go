#include <set>
#include <vector>

#include "zero/board.h"
#include "zero/game.h"

namespace go_zero {

bool Game::Play(const Move& move) {
  if (TryPlay(move)) {
    ++moves_played_;
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
        winner_ = (EvaluateScore() > 0 ? Color::kBlack : Color::kWhite);
      }
      return true;
    }
    case Move::kPlayStone: {
      if (board_.PlayStone(move.pos, curr_player_)) {
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
