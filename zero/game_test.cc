#include <vector>

#include "zero/game.h"

#include "gtest/gtest.h"

namespace go_zero {

TEST(GameTest, Score) {
  Game g(7, 7, 0);
  EXPECT_FALSE(g.IsGameOver());

  // Black
  EXPECT_TRUE(g.Play(Move(Move::kPlayStone, 0, 1)));
  // White
  EXPECT_TRUE(g.Play(Move(Move::kPass)));
  // Black
  EXPECT_TRUE(g.Play({Move::kPlayStone, 1, 0}));
  // White
  EXPECT_TRUE(g.Play({Move::kPlayStone, 4, 4}));
  // Black
  EXPECT_TRUE(g.Play(Move(Move::kPass)));
  // White
  EXPECT_TRUE(g.Play(Move(Move::kPass)));

  EXPECT_TRUE(g.IsGameOver());
  EXPECT_EQ(1, g.score());
}

TEST(GameTest, Capture) {
}

}  // namespace go_zero
