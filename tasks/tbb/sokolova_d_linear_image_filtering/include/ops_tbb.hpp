// Copyright 2024 Sokolova Daria
#pragma once

#include <tbb/tbb.h>

#include <cmath>
#include <cstring>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"

using std::vector;

namespace sokolova_tbb {

struct Color {
  uint8_t R, G, B;
};

class GaussFilterHorizontalTbb : public ppc::core::Task {
 public:
  explicit GaussFilterHorizontalTbb(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  uint8_t *input, *output;
  uint32_t width, height;
  vector<Color> image;

  static const size_t kSize = 3;
  double kernel[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  void makeKernel(float sigma = 2);
  void applyKernel();
  Color calculateNewPixelColor(size_t x, size_t y);
};

}  // namespace sokolova_tbb
