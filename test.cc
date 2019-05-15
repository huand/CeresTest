// Copyright 2019 hubert

#include <iostream>
#include <vector>

int main(int argc, char const *argv[]) {
  double d = 4;
  double *const p = &d;
  *p = 1000;
  std::cout << d << std::endl;
  return 0;
}
