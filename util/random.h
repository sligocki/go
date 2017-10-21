#include <random>

class Random {
 public:
  Random() : seed_(std::random_device()()), random_(seed_()) {}

  double UniformReal(int low, int high) {
    std::uniform_real_distribution<> dist(low, high);
    return dist(random_engine_);
  }

 private:
  uint32 seed_;
  std::mt19937 random_engine_;
};
