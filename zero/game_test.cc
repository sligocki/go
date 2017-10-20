#include <vector>

#include "zero/game.h"

#include "gtest/gtest.h"

namespace go_zero {

TEST(GameTest, Score) {
  Game g(7, 7, 0);
  EXPECT_FALSE(g.IsGameOver());

  // Black
  EXPECT_TRUE(g.Play(Move(Move::kPlayStone, Pos(0, 1))));
  // White
  EXPECT_TRUE(g.Play(Move(Move::kPass)));
  // Black
  EXPECT_TRUE(g.Play(Move(Move::kPlayStone, Pos(1, 0))));
  // White
  EXPECT_TRUE(g.Play(Move(Move::kPlayStone, Pos(4, 4))));
  // Black
  EXPECT_TRUE(g.Play(Move(Move::kPass)));
  // White
  EXPECT_TRUE(g.Play(Move(Move::kPass)));

  EXPECT_TRUE(g.IsGameOver());
  // Black has 2 stones + Pos(0, 0) as territory (3 points).
  // White has 1 stone and no territory (1 point).
  EXPECT_EQ(2, g.score());
  EXPECT_EQ(Color::kBlack, g.winner());
}

TEST(GameTest, Capture) {
}

}  // namespace go_zero
