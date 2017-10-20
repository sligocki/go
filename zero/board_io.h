#ifndef GO_ZERO_BOARD_IO_H
#define GO_ZERO_BOARD_IO_H

#include <string>

#include "board.h"

namespace go_zero {

// Parse board from ASCII format.
bool StringToBoard(const std::string& board_str, Board* board);

// Serialize board into ASCII format.
std::string BoardToString(const Board& board);

}  // namespace go_zero

#endif  // GO_ZERO_BOARD_IO_H
