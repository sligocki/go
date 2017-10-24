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
DEFINE_int64(mcts_searches_per_move, 1600, "Number of MCTS searches per move.");
DEFINE_double(mcts_decay, 0.9,
              "How much to decay each action value after a visit.");

namespace go_zero {

void Main() {
  // Play on every position twice.
  int max_moves = FLAGS_board_size * FLAGS_board_size * 2;
  UniformPlayer player;
  Game game(FLAGS_board_size, FLAGS_board_size, FLAGS_komi);
  MCTS mcts(FLAGS_mcts_searches_per_move, max_moves, FLAGS_mcts_decay,
            game, &player);
  mcts.SelfPlayGame();
}

}  // namespace go_zero

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  go_zero::Main();
}
