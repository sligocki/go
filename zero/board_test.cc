#include <string>

#include "zero/board.h"
#include "zero/board_io.h"

#include "gtest/gtest.h"

namespace go_zero {

TEST(BoardTest, BoardToString) {
  Board board(5, 3);

  EXPECT_EQ(".....\n"
            ".....\n"
            ".....\n", BoardToString(board));

  board.SetPos(Pos(2, 1), Color::kWhite);
  
  EXPECT_EQ(".....\n"
            "..O..\n"
            ".....\n", BoardToString(board));

  board.SetPos(Pos(0, 0), Color::kBlack);
  board.SetPos(Pos(0, 1), Color::kBlack);
  
  EXPECT_EQ("X....\n"
            "X.O..\n"
            ".....\n", BoardToString(board));
}

TEST(BoardTest, IO) {
  Board board(3, 3);
  std::string board_str =
    "XOX\n"
    "OXO\n"
    "XOX\n";
  EXPECT_TRUE(StringToBoard(board_str, &board));
  EXPECT_EQ(board_str, BoardToString(board));

  // TODO: Randomly check several boards.
}

TEST(BoardTest, ScoreSimple) {
  Board board(5, 5);
  EXPECT_EQ(0, board.Score());

  // All empty positions can only reach kBlack.
  board.SetPos(Pos(2, 2), Color::kBlack);
  EXPECT_EQ(25, board.Score());

  // All empty positions can reach both kBlack and kWhite.
  board.SetPos(Pos(0, 0), Color::kWhite);
  EXPECT_EQ(0, board.Score());

  // All empty positions can only reach kWhite.
  board.SetPos(Pos(2, 2), Color::kNone);
  EXPECT_EQ(-25, board.Score());
}

TEST(BoardTest, ScoreComplex) {
  Board board(5, 5);
  EXPECT_TRUE(StringToBoard("..XO.\n"
                            "..XO.\n"
                            "XXOO.\n"
                            "OOO..\n"
                            "X....\n", &board));
  // Note: Large territory on right & bottom doesn't score because White needs
  // to capture first.
  EXPECT_EQ(9 - 7, board.Score());

  board.SetPos(Pos(2, 4), Color::kWhite);
  EXPECT_EQ("..XO.\n"
            "..XO.\n"
            "XXOO.\n"
            "OOO..\n"
            "X.O..\n", BoardToString(board));
  // Now Left territory does count (bottom left still isn't resolved).
  EXPECT_EQ(9 - 15, board.Score());
}

TEST(BoardTest, Capture) {
  Board board(5, 5);
  EXPECT_TRUE(StringToBoard("XO...\n"
                            ".....\n"
                            ".....\n"
                            ".....\n"
                            ".....\n", &board));
  EXPECT_EQ(1 - 1, board.Score());

  // Captures one stone in corner.
  board.PlayStone(Pos(0, 1), Color::kWhite);
  EXPECT_EQ(".O...\n"
            "O....\n"
            ".....\n"
            ".....\n"
            ".....\n", BoardToString(board));
  EXPECT_EQ(-25, board.Score());


  EXPECT_TRUE(StringToBoard(".....\n"
                            ".....\n"
                            ".OXO.\n"
                            "..O..\n"
                            ".....\n", &board));
  EXPECT_EQ(1 - 3, board.Score());

  // Captures one stone in center.
  board.PlayStone(Pos(2, 1), Color::kWhite);
  EXPECT_EQ(".....\n"
            "..O..\n"
            ".O.O.\n"
            "..O..\n"
            ".....\n", BoardToString(board));
  EXPECT_EQ(-25, board.Score());
}

}  // namespace go_zero

