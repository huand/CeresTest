#include <iostream>
#include "ceres/ceres.h"
#include "glog/logging.h"
struct CostFunctor {
  template <typename T>
  bool operator()(const T *const x, T *res) const {
    res[0] = static_cast<T>(10) - x[0];
    return true;
  }
};

int main(int argc, char const *argv[]) {
  ceres::Solver::Options options;
  options.minimizer_progress_to_stdout = true;
  options.linear_solver_type = ceres::DENSE_QR;
  ceres::Problem problem;
  ceres::CostFunction *cost_function =
      new ceres::AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);
  double x0 = 100;
  double x = x0;
  problem.AddResidualBlock(cost_function, nullptr, &x);
  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem, &summary);
  std::cout << summary.FullReport() << std::endl;
  return 0;
}
