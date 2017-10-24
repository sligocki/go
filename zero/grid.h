#ifndef GO_ZERO_GRID_H
#define GO_ZERO_GRID_H

#include <vector>

namespace go_zero {

// Simple wrapper around vector<vector<>>, but it simplifies a lot of
// boilerplate that I have to put in multiple places otherwise.
template <typename T>
class Grid {
 public:
  Grid(int width, int height)
    : width_(width), height_(height) {
    for (int x = 0; x < width; ++x) {
      data_.emplace_back(height);
    }
  }

  T& Get(int x, int y) { return data_[x][y]; }
  const T& Get(int x, int y) const { return data_[x][y]; }

  int width() const { return width_; }
  int height() const { return height_; }

 private:
  const int width_ = 0;
  const int height_ = 0;
  std::vector<std::vector<T>> data_;
};

}  // namespace go_zero

#endif  // GO_ZERO_GRID_H
