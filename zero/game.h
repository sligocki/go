#ifndef GO_ZERO_GAME_H
#define GO_ZERO_GAME_H

#include <set>
#include <vector>

#include "zero/board.h"

namespace go_zero {

struct Move {
  enum Type {
    kPlayStone,
    kPass,
    kResign,
  };

  explicit Move(Type type) : type(type), pos(0, 0) {}
  Move(Type type, Pos pos) : type(type), pos(pos) {}

  Type type = kPass;
  Pos pos;
};

// Stores game state and 
class Game {
 public:
  Game(int board_width, int board_height, int komi)
    : komi_(komi), board_(board_width, board_height) {
  }

  // Returns true if move is legal and was applied.
  bool Play(const Move& move);

  bool IsGameOver() const { return winner_ != Color::kNone; }
  Color winner() const { return winner_; }
  int score() const { return score_; }

  // TODO: return input board state features used by Neural Network.
  void GetFeatures() const;

 private:
  bool TryPlay(const Move& move);

  // White wins ties so -> komi = komi_ + 0.5
  const int komi_ = 0;

  Board board_;
  // TODO: Deal with KO and maybe store last N (8) boards.
  Color curr_player_ = Color::kBlack;
  // Number of sequential passes, when it reaches 2, game is over.
  int num_passes_ = 0;
  Color winner_ = Color::kNone;
  int score_ = 0;

  int stones_played_ = 0;
};

}  // namespace go_zero

#endif  // GO_ZERO_GAME_H
