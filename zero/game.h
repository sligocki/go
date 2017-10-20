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

struct Pos {
  Pos(int x, int y) : x(x), y(y) {}
  int x = 0;
  int y = 0;
};

// Just the board vector. Caller must evaluate validity of board position.
class Board {
 public:
  Board(int width, int height)
    : width_(width), height_(height),
      board_(width, std::vector<Color>(height, Color::kNone)) {
  }

  Color GetPos(Pos p) const {
    return board_[p.x][p.y];
  }

  void SetPos(Pos p, Color cont) {
    board_[p.x][p.y] = cont;
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
  bool IsOnBoard(const Pos& pos) const;
  std::vector<Pos> GetNeighbors(const Pos& pos) const;

  bool TryPlay(const Move& move);
  bool PlayStone(const Pos& pos);
  void TryLift(const Pos& pos);
  void ScoreBoard();

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
