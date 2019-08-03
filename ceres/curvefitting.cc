// Copyright 2019 Hubert andre
#include "ceres/ceres.h"
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

struct XY {
  double x;
  double y;
};

struct CostFunctor {
  double x;
  double y;
  template <typename T>
  bool operator()(const T *const m, const T *const c, T *res) const {
    res[0] = (T)y - exp(m[0] * (T)x + c[0]);
    return true;
  };
};

int main(int argc, char const *argv[]) {
  //////////////////////////////////////
  // get input data
  std::vector<XY> xy;
  {
    std::ifstream in("XY.csv");
    double x, y;
    char c;
    while (in >> x >> c >> y) {
      xy.push_back(XY{x, y});
    }
  }
  //////////////////////////////////////
  double m = 0;
  double c = 0;

  ceres::Problem problem;
  CostFunctor *cfp = new CostFunctor{1.2, 30};
  for (int i = 0; i < xy.size(); i++) {
    ceres::CostFunction *costfunction =
        new ceres::AutoDiffCostFunction<CostFunctor, 1, 1, 1>(
            new CostFunctor{xy[i].x, xy[i].y});
    problem.AddResidualBlock(costfunction, NULL, &m, &c);
  }

  ceres::Solver::Summary summary;
  ceres::Solver::Options options;
  ceres::Solve(options, &problem, &summary);
  std::cout << summary.FullReport() << std::endl;
  std::cout << "m: " << m << std::endl;
  std::cout << "c: " << c << std::endl;

  std::ofstream out("mc.csv");
  out << m << ", " << c << '\n';
  return 0;
}
