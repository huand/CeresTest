#include <algorithm>
#include <fstream>
#include <iostream>
#include "ceres/ceres.h"
#include "ceres/rotation.h"
#include "glog/logging.h"

struct Projector {
  Projector(double x, double y) : px(x), py(y){};
  template <typename T>
  bool operator()(const T* const xp, const T* const xc, const T* const q,
                  const T* const f, const T* const k, T* res) const {
    T r[3];
    ceres::AngleAxisRotatePoint(q, xp, r);
    r[0] += xc[0];
    r[1] += xc[1];
    r[2] += xc[2];
    T dx = -r[0] / r[2];
    T dy = -r[1] / r[2];
    T R2 = pow(dx, 2) + pow(dy, 2);
    T distortion = T(1.0) + R2 * k[0] + k[1] * pow(R2, 2);
    res[0] = dx * f[0] * distortion - px;
    res[1] = dy * f[0] * distortion - py;
    return true;
  };

 private:
  double px;
  double py;
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
  Point(double d0, double d1, double d2) {
    x[0] = d0;
    x[1] = d1;
    x[2] = d2;
  }
  double x[3] = {1, 1, 1};
};

struct Rot {
  Rot(double d0, double d1, double d2) {
    r[0] = d0;
    r[1] = d1;
    r[2] = d2;
  }
  double r[3];
};

struct Distortion {
  Distortion(double k1, double k2) {
    k[0] = k1;
    k[1] = k2;
  };
  double k[2];
};

int main(int argc, char const* argv[]) {
  google::InitGoogleLogging(argv[0]);
  std::vector<DataRow> data;
  int maxframe, maxpoint, maxobs;

  std::ifstream in(
      "/home/home/Documents/datasets/bal/ladybug/problem-49-7776-pre.txt");
  in >> maxframe >> maxpoint >> maxobs;
  std::vector<Point> point;  //(maxpoint, Point{});
  std::vector<Rot> rot;      //(maxframe, Rot{});
  std::vector<Point> cam;    //(maxobs, Point{});
  std::vector<double> focal;
  std::vector<Distortion> distortion;
  int keyframe, pt;
  double x, y;
  Pixel pixel;
  for (int k = 0; k < maxobs; k++) {
    in >> keyframe >> pt >> x >> y;
    data.push_back(DataRow{keyframe, pt, Pixel{x, y}});
  }
  for (int k = 0; k < 9 * maxframe; k++) {
    double d1, d2, d3;
    in >> d1 >> d2 >> d3;
    rot.push_back(Rot{d1, d2, d3});
    in >> d1 >> d2 >> d3;
    cam.push_back(Point{d1, d2, d3});
    in >> d1;
    focal.push_back(d1);
    in >> d1 >> d2;
    distortion.push_back(Distortion{d1, d2});
  }
  for (int i = 0; i < maxpoint; i++) {
    double d1, d2, d3;
    in >> d1 >> d2 >> d3;
    point.push_back(Point{d1, d2, d3});
  }

  ceres::Solver::Options options;
  options.max_num_iterations = 2000;
  ceres::Problem problem;
  ceres::Solver::Summary summary;
  double f = 500;
  double k[] = {0, 0};
  for (int i = 0; i < data.size(); i++) {
    ceres::CostFunction* costfuntion =
        new ceres::AutoDiffCostFunction<Projector, 2, 3, 3, 3, 1, 2>(
            new Projector{data[i].pixel.x, data[i].pixel.y});
    int ip = data[i].point;
    int ic = data[i].keyframe;
    double res[2];
    problem.AddResidualBlock(costfuntion, NULL, point[ip].x, cam[ic].x,
                             rot[ic].r, &focal[ic], distortion[ic].k);
  }

  options.minimizer_progress_to_stdout = true;
  ceres::Solve(options, &problem, &summary);
  std::cout << summary.FullReport() << std::endl;

  std::ofstream out("xyz.csv");
  for (int i = 0; i < data.size(); i++) {
    out << point[i].x[0] << "\t" << point[i].x[1] << "\t" << point[i].x[2]
        << '\n';
  }
  std::cout << f << std::endl;
  return 0;
}
