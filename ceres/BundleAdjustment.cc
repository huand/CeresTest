#include <algorithm>
#include <fstream>
#include <iostream>
#include "ceres/ceres.h"
#include "ceres/rotation.h"
#include "glog/logging.h"

struct Projector {
  Projector(double x, double y) : px(x), py(y){};
  template <typename T>
  bool operator()(const T* const xp,
                  const T* const xc,
                  const T* const rot,
                  const T* const f,
                  const T* const k,
                  T* res) const {
    T r[3];
    ceres::AngleAxisRotatePoint(rot, xp, r);
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
struct Projector0 {
  // Projector0(double x, double y) : px(x), py(y){};
  template <typename T>
  bool operator()(const T* const xp,
                  const T* const f,
                  const T* const k,
                  T* res) const {
    T r[3];
    T Rot[3] = {(T)rot[0], (T)rot[1], (T)rot[2]};
    ceres::AngleAxisRotatePoint(Rot, xp, r);
    r[0] += xc[0];
    r[1] += xc[1];
    r[2] += xc[2];
    T dx = -r[0] / r[2];
    T dy = -r[1] / r[2];
    T R2 = pow(dx, 2) + pow(dy, 2);
    T distortion = T(1.0) + R2 * k[0] + k[1] * pow(R2, 2);
    res[0] = dx * f[0] * distortion - pix[0];
    res[1] = dy * f[0] * distortion - pix[1];
    return true;
  };
  // private:
  double pix[2];
  double xc[3];
  double rot[3];
};

struct DataRow {
  int keyframe;
  int point;
  double pixel[2];
};

struct Keyframe {
  double rot[3];
  double pos[3];
  double focal;
  double distortion[2];
};
struct Point {
  double x[3];
};

int main(int argc, char const* argv[]) {
  google::InitGoogleLogging(argv[0]);
  std::vector<DataRow> data;
  int maxframe, maxpoint, maxobs;
  std::ifstream in(
      "/home/adlink/Documents/datasets/bal/ladybug/problem-138-19878-pre.txt");
  in >> maxframe >> maxpoint >> maxobs;
  std::vector<Keyframe> keyframes;
  int kf, pt;
  double x, y;
  for (int k = 0; k < maxobs; k++) {
    in >> kf >> pt >> x >> y;
    data.push_back(DataRow{kf, pt, {x, y}});
  }
  for (int k = 0; k < maxframe; k++) {
    double r1, r2, r3, p1, p2, p3, f, k1, k2;
    in >> r1 >> r2 >> r3;
    in >> p1 >> p2 >> p3;
    in >> f >> k1 >> k2;
    keyframes.push_back(Keyframe{{r1, r2, r3}, {p1, p2, p3}, f, {k1, k2}});
  }
  std::vector<Point> points;
  for (int i = 0; i < maxpoint; i++) {
    double px, py, pz;
    in >> px >> py >> pz;
    points.push_back(Point{{px, py, pz}});
  }
  double p0[3] = {keyframes[0].pos[0], keyframes[0].pos[1],
                  keyframes[0].pos[2]};
  double r0[3] = {keyframes[0].rot[0], keyframes[0].rot[1],
                  keyframes[0].rot[2]};
  // Projector0 ppp = Projector0{{123, 321}, {1., 2., 3.}, {11., 22., 33.}};
  ceres::Problem problem;
  ceres::Solver::Summary summary;
  for (int i = 0; i < data.size(); i++) {
    int ic = data[i].keyframe;
    int ip = data[i].point;
    double res[2];
    ceres::CostFunction* costfuntion;
    if (ic != 0) {
      costfuntion =
          new ceres::AutoDiffCostFunction<Projector, 2, 3, 3, 3, 1, 2>(
              new Projector{data[i].pixel[0], data[i].pixel[1]});
      problem.AddResidualBlock(costfuntion, NULL, points[ip].x,
                               keyframes[ic].pos, keyframes[ic].rot,
                               &keyframes[ic].focal, keyframes[ic].distortion);
    } else {
      costfuntion = new ceres::AutoDiffCostFunction<Projector0, 2, 3, 1, 2>(
          new Projector0{data[i].pixel[0],
                         data[i].pixel[1],
                         {p0[0], p0[1], p0[2]},
                         {r0[0], r0[1], r0[2]}});
      problem.AddResidualBlock(costfuntion, NULL, points[ip].x,
                               &keyframes[ic].focal, keyframes[ic].distortion);
      // costfuntion =
      //     new ceres::AutoDiffCostFunction<Projector, 2, 3, 3, 3, 1, 2>(
      //         new Projector{data[i].pixel[0], data[i].pixel[1]});
      // problem.AddResidualBlock(costfuntion, NULL, points[ip].x,
      //                          keyframes[ic].pos, keyframes[ic].rot,
      //                          &keyframes[ic].focal,
      //                          keyframes[ic].distortion);
    }
  }

  ceres::Solver::Options options;
  options.max_num_iterations = 2000;
  options.minimizer_progress_to_stdout = true;
  options.linear_solver_type = ceres::DENSE_SCHUR;
  ceres::Solve(options, &problem, &summary);
  std::cout << summary.FullReport() << std::endl;

  std::ofstream out("xyz.csv");
  for (int i = 0; i < points.size(); i++) {
    out << points[i].x[0] << "\t" << points[i].x[1] << "\t" << points[i].x[2]
        << '\n';
  }
  return 0;
}
