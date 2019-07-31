#include <iostream>
#include "ceres/ceres.h"
#include "glog/logging.h"

struct CostFunctor1 {
  template <typename T>
  bool operator()(const T* const x1, const T* const x2, T* residual) const {
    residual[0] = x1[0] + (T)10 * x2[0];
    return true;
  }
};
struct CostFunctor2 {
  template <typename T>
  bool operator()(const T* const x3, const T* const x4, T* residual) const {
    residual[0] = sqrt(5) * (x3[0] - x4[0]);
    return true;
  }
};
struct CostFunctor3 {
  template <typename T>
  bool operator()(const T* const x2, const T* const x3, T* residual) const {
    residual[0] = pow(x2[0] - (T)2 * x3[0], 2);
    return true;
  }
};
struct CostFunctor4 {
  template <typename T>
  bool operator()(const T* const x1, const T* const x4, T* residual) const {
    residual[0] = sqrt(10) * pow(x1[0] - x4[0], 2);
    return true;
  }
};

int main(int argc, char const* argv[]) {
  ceres::Problem problem;
  ceres::CostFunction* costfunction1 =
      new ceres::AutoDiffCostFunction<CostFunctor1, 1, 1, 1>(new CostFunctor1);
  ceres::CostFunction* costfunction2 =
      new ceres::AutoDiffCostFunction<CostFunctor2, 1, 1, 1>(new CostFunctor2);
  ceres::CostFunction* costfunction3 =
      new ceres::AutoDiffCostFunction<CostFunctor3, 1, 1, 1>(new CostFunctor3);
  ceres::CostFunction* costfunction4 =
      new ceres::AutoDiffCostFunction<CostFunctor4, 1, 1, 1>(new CostFunctor4);
  double x1 = 3;
  double x2 = -1;
  double x3 = 0;
  double x4 = 1;
  problem.AddResidualBlock(costfunction1, nullptr,
                           std::vector<double*>{&x1, &x2});
  problem.AddResidualBlock(costfunction2, nullptr,
                           std::vector<double*>{&x3, &x4});
  problem.AddResidualBlock(costfunction3, nullptr,
                           std::vector<double*>{&x2, &x3});
  problem.AddResidualBlock(costfunction4, nullptr,
                           std::vector<double*>{&x1, &x4});
  ceres::Solver::Options options;
  ceres::Solver::Summary summary;
  options.minimizer_progress_to_stdout = true;
  ceres::Solve(options, &problem, &summary);
  std::cout << "x1: " << x1 << '\n';
  std::cout << "x2: " << x2 << '\n';
  std::cout << "x3: " << x3 << '\n';
  std::cout << "x4: " << x4 << '\n';
  std::cout << summary.FullReport() << std::endl;
  std::cout << summary.IsSolutionUsable() << '\n';
  return 0;
}
