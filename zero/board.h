#ifndef GO_ZERO_BOARD_H
#define GO_ZERO_BOARD_H

#include <set>
#include <vector>

#include "zero/grid.h"

namespace go_zero {

enum class Color {
  kNone = 0,  // Make sure this is default value.
  kBlack,
  kWhite,
};

inline Color OppositeColor(Color color) {
  switch (color) {
    case Color::kBlack:
      return Color::kWhite;
    case Color::kWhite:
      return Color::kBlack;
    case Color::kNone:
      return Color::kNone;
  }
}

struct Pos {
  Pos(int x, int y) : x(x), y(y) {}
  int x = 0;
  int y = 0;
};

struct PosComp {
  bool operator() (const Pos& a, const Pos& b) const {
    return std::pair<int, int>(a.x, a.y) < std::pair<int, int>(b.x, b.y);
  }
};

typedef std::set<Pos, PosComp> PosSet;


// Just the board vector. Caller must evaluate validity of board position.
class Board {
 public:
  Board(int width, int height)
    : board_(width, height) {
  }

  bool PlayStone(const Pos& pos, Color color);

  // Return score on board (without komi) using Tromp-Taylor rules.
  // > 0 means Black has more points. < 0 means White has more points.
  int Score() const;

  Color GetPos(const Pos& p) const {
    return board_.Get(p.x, p.y);
  }

  void SetPos(const Pos& p, Color color) {
    board_.Get(p.x, p.y) = color;
  }

  int width() const { return board_.width(); }
  int height() const { return board_.height(); }

 private:
  bool IsOnBoard(const Pos& pos) const;
  std::vector<Pos> GetNeighbors(const Pos& pos) const;

  void FindReachable(const Pos& pos, Color color,
                     PosSet* string, std::set<Color>* colors_reachable) const;
  bool IsTerritory(const Pos& pos, PosSet* string, Color* color) const;

  void TryLift(const Pos& init_pos, Color init_color);

  Grid<Color> board_;
};

}  // namespace go_zero

#endif  // GO_ZERO_BOARD_H
