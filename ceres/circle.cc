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
                  const T* const rodrigues, T* res) const {
    T u[] = {(T)0., (T)0., (T)1.};
    T n[3];
    ceres::AngleAxisRotatePoint((T*)rodrigues, u, n);
    T cross[3];
    T xc[] = {xyz[0] - center[0], xyz[1] - center[1], xyz[2] - center[2]};
    ceres::CrossProduct(n, xc, cross);
    ceres::CrossProduct(cross, n, u);
    T length = sqrt(ceres::DotProduct(u, u));
    T v[] = {radius[0] * u[0] / length, radius[0] * u[1] / length,
             radius[0] * u[2] / length};
    res[0] = xc[0] - v[0];
    res[1] = xc[1] - v[1];
    res[2] = xc[2] - v[2];
    return true;
  }
  double xyz[3];
};

int main(int argc, char const* argv[]) {
  CircleData circle("data/circle_noisy.csv");
  ceres::Solver::Options options;
  ceres::Solver::Summary summary;
  ceres::Problem problem;
  double radius = 1;
  double center[] = {0, 0, 0};
  double rodrigues[] = {0, 0, 0};
  /////////////////
  Distancor dor{20, 0, 0};
  double res[3];
  dor(&radius, center, rodrigues, res);
  /////////////////
  for (int i = 0; i < circle.points_.size(); i++) {
    ceres::CostFunction* cost_function =
        new ceres::AutoDiffCostFunction<Distancor, 3, 1, 3, 3>(
            new Distancor{{circle.points_[i].xyz[0], circle.points_[i].xyz[1],
                           circle.points_[i].xyz[2]}});
    problem.AddResidualBlock(cost_function, nullptr, &radius, center,
                             rodrigues);
  }
  ceres::Solve(options, &problem, &summary);
  std::cout << summary.FullReport();
  std::cout << "r: " << radius << '\n';
  std::cout << "x: " << center[0] << '\n';
  std::cout << "y: " << center[1] << '\n';
  std::cout << "z: " << center[2] << '\n';
  return 0;
}
