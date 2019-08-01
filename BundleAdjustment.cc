#include <algorithm>
#include <fstream>
#include <iostream>
#include "ceres/ceres.h"
#include "glog/logging.h"

template <typename T>  // todo no ponintert plss
bool QuatMult(const T* a1, const T* b1, const T* c1, const T* d1, const T* a2,
              const T* b2, const T* c2, const T* d2, T* a3, T* b3, T* c3,
              T* d3) {
  a3[0] = a1[0] * a2[0] - b1[0] * b2[0] - c1[0] * c2[0] - d1[0] * d2[0];
  b3[0] = a1[0] * b2[0] + b1[0] * a2[0] + c1[0] * d2[0] - d1[0] * c2[0];
  c3[0] = a1[0] * c2[0] - b1[0] * d2[0] + c1[0] * a2[0] + d1[0] * b2[0];
  d3[0] = a1[0] - d2[0] + b1[0] * c2[0] - c1[0] * b2[0] + d1[0] * a2[0];
  return true;
}

struct Projector {
  Projector(double x, double y) : px(x), py(y){};
  template <typename T>
  bool operator()(const T* const xp, const T* const yp, const T* const zp,
                  const T* const xc, const T* const yc, const T* const zc,
                  const T* const q0, const T* const q1, const T* const q2,
                  // TODO: add disto
                  T* res) const {
    T q3 = sqrt(pow(q0[0], 2) + pow(q1[0], 2) + pow(q2[0], 2));
    T xr = xp[0] - xc[0];
    T yr = yp[0] - yc[0];
    T zr = zp[0] - zc[0];
    T r0, r1, r2, r3, r10, r11, r12, r13;
    T z{};
    QuatMult(q0, q1, q2, &q3, &z, &xr, &yr, &zr, &r0, &r1, &r2, &r3);
    T mq1 = -q1[0];
    T mq2 = -q2[0];
    T mq3 = -q3;
    QuatMult(&r0, &r1, &r2, &r3, q0, &mq1, &mq2, &mq3, &r10, &r11, &r12, &r13);
    T dx, dy;
    dx = -r11 / r13 * f;
    dy = -r12 / r13 * f;
    res[0] = dx - px;
    res[1] = dy - py;
    return true;
  };

 private:
  double px;
  double py;
  double f = 500;
};
struct Pixel {
  double x;
  double y;
};

struct DataRow {
  int keyframe;
  int point;
  Pixel pixel;
};

struct Point {
  double x, y, z;
};

struct Quat {
  double q0, q1, q2, q3;
};

int main(int argc, char const* argv[]) {
  google::InitGoogleLogging(argv[0]);
  std::vector<DataRow> data;
  {
    std::ifstream in(
        "/home/home/Documents/datasets/bal/ladybug/problem-49-7776-pre.txt");
    int i;
    in >> i >> i >> i;
    int keyframe, point;
    double x, y;
    Pixel pixel;
    while (in >> keyframe >> point >> x >> y) {
      data.push_back(DataRow{keyframe, point, Pixel{x, y}});
    }
  }

  std::vector<Point> point(data.size(), Point{0, 0, 0});
  std::vector<Quat> quat(data.size(), Quat{1, 0, 0});
  std::vector<Point> cam(data.size(), Point{0, 0, 0});

  ceres::Solver::Options options;
  options.max_num_iterations = 2000;
  ceres::Problem problem;
  ceres::Solver::Summary summary;

  for (int i = 0; i < data.size(); i++) {
    ceres::CostFunction* costfuntion =
        new ceres::AutoDiffCostFunction<Projector, 2, 1, 1, 1, 1, 1, 1, 1, 1,
                                        1>(
            new Projector{data[i].pixel.x, data[i].pixel.y});
    int ip = data[i].point;
    int ic = data[i].keyframe;
    problem.AddResidualBlock(costfuntion, new ceres::CauchyLoss(0.5),
                             &point[ip].x, &point[ip].y, &point[ip].z,
                             &cam[ic].x, &cam[ic].y, &cam[ic].z, &quat[ic].q0,
                             &quat[ic].q1, &quat[ic].q2);
  }

  options.minimizer_progress_to_stdout = true;
  ceres::Solve(options, &problem, &summary);
  std::cout << summary.FullReport() << std::endl;

  std::ofstream out("xyz.csv");
  for (int i = 0; i < data.size(); i++) {
    out << point[i].x << "\t" << point[i].y << "\t" << point[i].z << '\n';
  }

  return 0;
}
