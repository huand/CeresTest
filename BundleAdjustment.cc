#include <fstream>
#include <iostream>
#include "ceres/ceres.h"

bool QuatMult(double* a1,
              double* b1,
              double* c1,
              double* d1,
              double* a2,
              double* b2,
              double* c2,
              double* d2,
              double* a3,
              double* b3,
              double* c3,
              double* d3) {
  a3[0] = a1[0] * a2[0] - b1[0] * b2[0] - c1[0] * c2[0] - d1[0] * d2[0];
  b3[0] = a1[0] * b2[0] + b1[0] * a2[0] + c1[0] * d2[0] - d1[0] * c2[0];
  c3[0] = a1[0] * c2[0] - b1[0] * d2[0] + c1[0] * a2[0] + d1[0] * b2[0];
  d3[0] = a1[0] - d2[0] + b1[0] * c2[0] - c1[0] * b2[0] + d1[0] * a2[0];
  return true;
}

struct Projector {
  template <typename T>
  bool operator()(const T* const xp,
                  const T* const yp,
                  const T* const zp,
                  const T* const xc,
                  const T* const yc,
                  const T* const zc,
                  const T* const q0,
                  const T* const q1,
                  const T* const q2,
                  const T* const f,  // TODO: add disto
                  T* res) const {
    double q3 = sqrt(pow(q0, 2) + pow(q1, 2) + pow(q2, 2));
    double xr = *xp - *xc;
    double yr = *yp - *yc;
    double zr = *zp - *zc;
    double r0, r1, r2, r3;
    double z = 0;
    QuatMult(q0, q1, q2, &q3, &z, &xr, &yr, &zr, &r0, &r1, &r2, &r3);
    QuatMult(&r0, &r1, &r2, &r3, -q0, q1, q2, &q3, &z, xc, yc, zc);
    xc[0] /= -xc[0] / zc[0];
    yc[0] /= -yc[0] / zc[0];
    res[0] = xc[0] - px;
    res[1] = yc[0] - py;
    return true;
  };

 private:
  double px;
  double py;
};

int main(int argc, char const* argv[]) {
  int x = 0;
  int* myx = new int();
  float z = 12;

  for (size_t i = 0; i < 12; i++) {
    std::cout << i << std::endl;
  }

  /* code */
  return 0;
}
