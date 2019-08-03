#include <fstream>
#include <iostream>
#include "ceres/ceres.h"
#include "ceres/rotation.h"

struct Point {
  double xyz[3];
};

struct CircleParam {
  double radius;
  Point center;
  Point rodrigues;
};

struct CircleData {
  CircleData(std::string path) {
    std::ifstream in(path);
    double x, y, z;
    while (in >> x >> y >> z) {
      points_.push_back(Point{{x, y, z}});
    }
  }
  std::vector<Point> points_;
};

struct Distancor {
  template <typename T>
  bool operator()(const T* const radius, const T* const center,
                  const T* rodrigues, T* res) {
    double u[] = {1, 0, 0};
    double n[3];
    ceres::AngleAxisRotatePoint(rodrigues, u, n);
    return true;
  }
  double xyz[3];
};

int main(int argc, char const* argv[]) {
  CircleData circle("data/circle_noisy.csv");

  return 0;
}
