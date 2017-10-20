#include "board_io.h"

#include <string>

#include "board.h"

namespace go_zero {

Color CharToColor(char c) {
  switch (c) {
    case 'X':
      return Color::kBlack;
    case 'O':
      return Color::kWhite;
    default:
      return Color::kNone;
  }
}

char ColorToChar(Color color) {
  switch (color) {
    case Color::kBlack:
      return 'X';
    case Color::kWhite:
      return 'O';
    case Color::kNone:
      return '.';
  }
}


// Parse board from ASCII format.
bool StringToBoard(const std::string& board_str, Board* board) {
  Pos pos(0, 0);
  for (char c : board_str) {
    if (c == '\n') {
      pos.y += 1;
      pos.x = 0;
    } else {
      board->SetPos(pos, CharToColor(c));
      pos.x += 1;
    }
  }
  // TODO: Fail for malformed strings.
  return true;
}

// Serialize board into ASCII format.
std::string BoardToString(const Board& board) {
  std::string board_str;
  for (int y = 0; y < board.height(); ++y) {
    for (int x = 0; x < board.width(); ++x) {
      board_str.push_back(ColorToChar(board.GetPos(Pos(x, y))));
    }
    board_str.push_back('\n');
  }
  return board_str;
}

}  // namespace go_zero
