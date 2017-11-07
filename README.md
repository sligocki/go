Exploration into AI for Game of Go.

### How to Build

1. Install [Bazel](https://www.bazel.build/)
2. `bazel test //...`

### How to run MCTS

It's not much yet, but you can run a rudimentary Monte Carlo Tree Search:

   ```
   bazel build //zero:mcts_main
   bazel-bin/zero/mcts_main --board_size=9
   ```

Right now, it just uses a completely random Player (chooses every move with
equal chance). But it demonstrates the MCTS algorithm of exploring a playing
tree, discounting paths that have already been taken and then choosing the
path that was explored the most.
