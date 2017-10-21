class Random;

// Runs single rollout and returns whether or not current player won.
bool MCRollout(const Game& starting_position, const Network& network,
               int max_moves, Random* random) {
  Game game(starting_position);
  while (!game.isGameOver() && game.moves_played() < max_moves) {
    // Get input board features (color of stone at every point, etc.)
    Features features;
    game.GetFeatures(&features);

    // Evaluate this in the Neural Network.
    Network::Result policy;
    network.Evaluate(features, &policy);

    // Keep trying to play moves until you play a legal one.
    // TODO: Will this cause problems? Maybe Pass if illegal move is chosen?
    bool successfully_moved = false;
    while (!successfully_moved) {
      // Randomly choose a move based on the policy.
      Move move = policy.RandomMove(random);
      successfully_moved = game.Play(move);
    }
  }

  return (game.winner() == starting_position.curr_player())
}
