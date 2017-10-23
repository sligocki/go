#include <vector>

#include "zero/board.h"

namespace go_zero {

// Feature of a single board position.
struct Feature {
  Color color = Color::kNone;
  // TODO: Add more.
};

// All features from the whole board.
struct Features {
  // TODO: Do we need these here? Or should we already know them in the player?
  int width = 0;
  int height = 0;
  std::vector<std::vector<Feature>> board_features;
};

}  // namespace go_zero
