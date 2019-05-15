#include <vector>
#include "ceres/ceres.h"
#include "gflags/gflags.h"
#include "glog/logging.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solve;
using ceres::Solver;

struct F1234 {
  template <typename T>
  bool operator()(const T* const x, T* residual) const {
    residual[0] = x[0] + 10.0 * x[1];
    residual[1] = sqrt(5.0) * (x[2] - x[3]);
    residual[2] = (x[1] - 2.0 * x[2]) * (x[1] - 2.0 * x[2]);
    residual[3] = sqrt(10.0) * (x[0] - x[3]) * (x[0] - x[3]);
    return true;
  }
};
DEFINE_string(minimizer, "trust_region",
              "Minimizer type to use, choices are: line_search & trust_region");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  
  double x[] = {3, -1, 0, 1};
  Problem problem;
  // Add residual terms to the problem using the using the autodiff
  // wrapper to get the derivatives automatically. The parameters, x1 through
  // x4, are modified in place.
  problem.AddResidualBlock(new AutoDiffCostFunction<F1234, 4, 4>(new F1234),
                           NULL, x);
  Solver::Options options;
  LOG_IF(FATAL, !ceres::StringToMinimizerType(FLAGS_minimizer,
                                              &options.minimizer_type))
      << "Invalid minimizer: " << FLAGS_minimizer
      << ", valid options are: trust_region and line_search.";
  options.max_num_iterations = 100;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = true;
  // std::cout << "Initial x1 = " << x1 << ", x2 = " << x2 << ", x3 = " << x3 <<
  // ", x4 = " << x4 << "\n"; Run the solver!
  Solver::Summary summary;
  Solve(options, &problem, &summary);
  std::cout << summary.FullReport() << "\n";
  // std::cout << "Final x1 = " << x1 << ", x2 = " << x2 << ", x3 = " << x3 <<
  // ", x4 = " << x4 << "\n";
  return 0;
}