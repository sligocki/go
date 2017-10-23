#include <inttypes.h>
#include <random>

class Random {
 public:
  Random() : seed_(std::random_device()()), random_engine_(seed_) {}

  double UniformReal(double low, double high) {
    std::uniform_real_distribution<double> dist(low, high);
    return dist(random_engine_);
  }

 private:
  uint32_t seed_;
  std::mt19937 random_engine_;

  // Disallow Copy and Assign
  Random(Random&) = delete;
  void operator=(Random) = delete;
};
