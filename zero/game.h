// TODO: Add header guards.

#include "zero/board.h"
#include "zero/color.h"

namespace go_zero {

struct Move {
  enum Type {
    kPlayStone,
    kPass,
    kResign,
  };
  Type type = kPass;

  int x = 0;
  int y = 0;
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

  // TODO: return input board state features used by Neural Network.
  void GetFeatures() const;

 private:
  // White wins ties so -> komi = komi_ + 0.5
  const int komi_ = 0;

  Board board_;
  // TODO: Deal with KO and maybe store last N (8) boards.
  Color curr_player_ = kBlack;
  // Number of sequential passes, when it reaches 2, game is over.
  int num_passes_ = 0;
  Color winner_ = kNone;

  int stones_played_ = 0;
};

}  // namespace go_zero
