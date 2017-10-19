#include "zero/color.h"

namespace go_zero {

// Just the board vector. Caller must evaluate validity of board position.
class Board {
 public:
  Board(int width, int height)
    : board_(width, std::vector<Color>(height, kEmpty)) {
  }

  PositionContents GetPos(int x, int y) const {
    return board_[x][y];
  }

  void SetPos(int x, int y, Color cont) {
    board_[x][y] = cont;
  }

 private:
  std::vector<std::vector<Color>> board_;
};

}  // namespace go_zero
