class Random;

namespace go_zero {

class Game;
class Player;

// Runs single rollout and returns whether or not current player won.
bool MCRollout(const Game& starting_position, const Player& player,
               int max_moves, Random* random);

double MCPercent(const Game& starting_position, const Player& player,
                 int max_moves, Random* random, int num_runs);

}  // namespace go_zero
