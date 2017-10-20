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
  EXPECT_TRUE(board.PlayStone(Pos(0, 1), Color::kWhite));
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
  EXPECT_TRUE(board.PlayStone(Pos(2, 1), Color::kWhite));
  EXPECT_EQ(".....\n"
            "..O..\n"
            ".O.O.\n"
            "..O..\n"
            ".....\n", BoardToString(board));
  EXPECT_EQ(-25, board.Score());


  EXPECT_TRUE(StringToBoard(".....\n"
                            "...X.\n"
                            ".XXOX\n"
                            ".XOOX\n"
                            "..OOX\n", &board));
  EXPECT_EQ(7 - 5, board.Score());

  // Captures a blob of stones on the edge.
  EXPECT_TRUE(board.PlayStone(Pos(1, 4), Color::kBlack));
  EXPECT_EQ(".....\n"
            "...X.\n"
            ".XX.X\n"
            ".X..X\n"
            ".X..X\n", BoardToString(board));
  EXPECT_EQ(25, board.Score());

  
  EXPECT_TRUE(StringToBoard(".....\n"
                            ".....\n"
                            "...XX\n"
                            "..XOO\n"
                            "..XO.\n", &board));
  EXPECT_EQ(21 - 4, board.Score());

  // Captures stones by taking their only eye.
  EXPECT_TRUE(board.PlayStone(Pos(4, 4), Color::kBlack));
  EXPECT_EQ(".....\n"
            ".....\n"
            "...XX\n"
            "..X..\n"
            "..X.X\n", BoardToString(board));
  EXPECT_EQ(25, board.Score());

  
  EXPECT_TRUE(StringToBoard(".....\n"
                            "..X..\n"
                            ".X.X.\n"
                            "..X..\n"
                            ".....\n", &board));

  // Captures stones by taking their only eye.
  EXPECT_TRUE(board.PlayStone(Pos(2, 2), Color::kWhite));
  // White suicide. No change.
  EXPECT_EQ(".....\n"
            "..X..\n"
            ".X.X.\n"
            "..X..\n"
            ".....\n", BoardToString(board));

  
  EXPECT_TRUE(StringToBoard("..O..\n"
                            ".OXO.\n"
                            ".X.XO\n"
                            "..XO.\n"
                            ".....\n", &board));

  // Captures stones by taking their only eye.
  EXPECT_TRUE(board.PlayStone(Pos(2, 2), Color::kWhite));
  // No longer suicide.
  EXPECT_EQ("..O..\n"
            ".O.O.\n"
            ".XO.O\n"
            "..XO.\n"
            ".....\n", BoardToString(board));
}

}  // namespace go_zero

