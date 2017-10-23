#include "zero/mcts.h"

#include <iostream>

#include <gflags/gflags.h>

#include "util/random.h"
#include "zero/board_io.h"
#include "zero/features.h"
#include "zero/game.h"
#include "zero/player.h"

DEFINE_int64(board_size, 19, "Go board size.");
DEFINE_int64(komi, 7, "Komi - 0.5.");
DEFINE_int64(mcts_num_runs, 10000, "Number of MCTS runs to simulate.");

namespace go_zero {

void Main() {
  Random random;
  Game game(FLAGS_board_size, FLAGS_board_size, FLAGS_komi);
  UniformPlayer player;
  // Play on every position twice.
  int max_moves = FLAGS_board_size * FLAGS_board_size * 2;
  while (!game.IsGameOver() && game.moves_played() < max_moves) {
    double max_prob = -1.0;
    Pos best_move(-1, -1);
    for (int x = 0; x < FLAGS_board_size; ++x) {
      for (int y = 0; y < FLAGS_board_size; ++y) {
        double prob = MCPercent(game, player, max_moves, &random,
                                FLAGS_mcts_num_runs);
        if (prob > max_prob) {
          max_prob = prob;
          best_move.x = x;
          best_move.y = y;
        }
      }
    }
    std::cout << "Max prob: " << max_prob << " x=" << best_move.x
              << " y=" << best_move.y << std::endl
              << BoardToString(game.board()) << std::endl;
    if (!game.Play(Move(Move::Type::kPlayStone, best_move))) {
      game.Play(Move(Move::Type::kPass));
    }
  }
}

}  // namespace go_zero

int main() {
  go_zero::Main();
}
