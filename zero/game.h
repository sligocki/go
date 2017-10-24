#ifndef GO_ZERO_GAME_H
#define GO_ZERO_GAME_H

#include <set>
#include <vector>

#include "zero/board.h"
#include "zero/grid.h"

namespace go_zero {

struct Features;

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

template <typename T>
class MoveMap {
 public:
  MoveMap(int width, int height)
    : play_(width, height), pass_(T()), resign_(T()) {}

  T& Get(const Move& move) {
    switch (move.type) {
      case Move::Type::kPlayStone:
        return play_.Get(move.pos.x, move.pos.y);
      case Move::Type::kPass:
        return pass_;
      case Move::Type::kResign:
        return resign_;
    }
  }

  const T& Get(const Move& move) const {
    switch (move.type) {
      case Move::Type::kPlayStone:
        return play_.Get(move.pos.x, move.pos.y);
      case Move::Type::kPass:
        return pass_;
      case Move::Type::kResign:
        return resign_;
    }
  }

  int width() const { return play_.width(); }
  int height() const { return play_.height(); }

 private:
  Grid<T> play_;
  T pass_;
  T resign_;
};

// Stores game state and 
class Game {
 public:
  Game(int board_width, int board_height, int komi)
    : komi_(komi), board_(board_width, board_height) {
  }

  // Returns true if move is legal and was applied.
  bool Play(const Move& move);

  // Get input board state features used by Neural Network.
  void GetFeatures(Features* features) const;

  // Evaluate score according to Tromp-Taylor rules. Can be evaluated at any
  // time, however it is not very meaningful until game is over.
  // If positive, number of points Black won by +0.5 (+4 -> Black won by 3.5).
  // If negative, number of points White won by -0.5 (-2 -> White won by 2.5).
  int EvaluateScore() const { return board_.Score() - komi_; }

  Color curr_player() const { return curr_player_; }
  int moves_played() const { return moves_played_; }

  bool IsGameOver() const { return winner_ != Color::kNone; }
  Color winner() const { return winner_; }

  const Board& board() const { return board_; }
  int width() const { return board_.width(); }
  int height() const { return board_.height(); }

 private:
  bool TryPlay(const Move& move);

  // White wins ties so -> komi = komi_ + 0.5
  const int komi_ = 0;

  Board board_;
  // TODO: Deal with KO and maybe store last N (8) boards.

  Color curr_player_ = Color::kBlack;
  // Number of sequential passes, when it reaches 2, game is over.
  int num_passes_ = 0;
  int moves_played_ = 0;

  Color winner_ = Color::kNone;
};

}  // namespace go_zero

#endif  // GO_ZERO_GAME_H
