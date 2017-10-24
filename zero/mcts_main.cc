// This is based on my "flawed" idea on how MCTS rollouts were done in this
// paper. After chatting with someone today, I realized that AG-0 doesn't
// actually use rollouts at all.

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
  std::cout << "FLAGS_board_size = " << FLAGS_board_size
            << " FLAGS_mcts_num_runs = " << FLAGS_mcts_num_runs << std::endl;
  Game game(FLAGS_board_size, FLAGS_board_size, FLAGS_komi);
  UniformPlayer player;
  // Play on every position twice.
  int max_moves = FLAGS_board_size * FLAGS_board_size * 2;
  while (!game.IsGameOver() && game.moves_played() < max_moves) {
    std::cout << BoardToString(game.board()) << std::endl;
    double max_prob = -1.0;
    Pos best_move(-1, -1);
    for (int x = 0; x < FLAGS_board_size; ++x) {
      for (int y = 0; y < FLAGS_board_size; ++y) {
        Game new_move(game);
        if (new_move.Play(Move(Move::Type::kPlayStone, Pos(x, y)))) {
          double prob = MCPercent(new_move, player, max_moves, &random,
                                  FLAGS_mcts_num_runs);
          if (prob > max_prob) {
            max_prob = prob;
            best_move.x = x;
            best_move.y = y;
          }
        }
      }
    }
    std::cout << "Max prob: " << max_prob << " x=" << best_move.x
              << " y=" << best_move.y << std::endl;
    if (!game.Play(Move(Move::Type::kPlayStone, best_move))) {
      std::cout << "Pass :(" << std::endl;
      game.Play(Move(Move::Type::kPass));
    }
  }
}

}  // namespace go_zero

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  go_zero::Main();
}
