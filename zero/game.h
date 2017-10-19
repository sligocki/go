// TODO: Add header guards.

#include <vector>

namespace go_zero {

enum class Color {
  kNone,
  kBlack,
  kWhite,
};

Color OppositeColor(Color color) {
  switch (color) {
    case Color::kBlack:
      return Color::kWhite;
    case Color::kWhite:
      return Color::kBlack;
    case Color::kNone:
      return Color::kNone;
  }
  return Color::kNone;
}

// Just the board vector. Caller must evaluate validity of board position.
class Board {
 public:
  Board(int width, int height)
    : width_(width), height_(height),
      board_(width, std::vector<Color>(height, Color::kNone)) {
  }

  Color GetPos(int x, int y) const {
    return board_[x][y];
  }

  void SetPos(int x, int y, Color cont) {
    board_[x][y] = cont;
  }

  int width() const { return width_; }
  int height() const { return height_; }

 private:
  const int width_ = 0;
  const int height_ = 0;
  std::vector<std::vector<Color>> board_;
};

struct Move {
  enum Type {
    kPlayStone,
    kPass,
    kResign,
  };

  Move(Type type, int x, int y) : type(type), x(x), y(y) {}

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
  bool IsOnBoard(const Move& pos) const;
  std::vector<Move> GetNeighbors(const Move& pos) const;

  bool TryPlay(const Move& move);
  void TryLift(const Move& pos);
  void ScoreBoard();

  // White wins ties so -> komi = komi_ + 0.5
  const int komi_ = 0;

  Board board_;
  // TODO: Deal with KO and maybe store last N (8) boards.
  Color curr_player_ = Color::kBlack;
  // Number of sequential passes, when it reaches 2, game is over.
  int num_passes_ = 0;
  Color winner_ = Color::kNone;

  int stones_played_ = 0;
};

}  // namespace go_zero
