// Copyright 2024 Platonova Mariya
#include "omp/platonova_m_jarvis_omp/include/ops_omp.hpp"

#include <omp.h>

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

std::vector<Point> Jarvis(const std::vector<Point>& Points) {
  if (Points.size() < 3) return Points;
  Point p0 = Points[0];
  for (const auto& p : Points) {
    if (p.x < p0.x || (p.x == p0.x && p.y < p0.y)) p0 = p;
  }
  std::vector<Point> convexHull = {p0};
  Point prevPoint = p0;
  while (true) {
    Point nextPoint = Points[0];
    for (const auto& point : Points) {
      if (point == prevPoint) continue;
      double crossProduct =
          (point.y - prevPoint.y) * (nextPoint.x - prevPoint.x) - (point.x - prevPoint.x) * (nextPoint.y - prevPoint.y);
      if (crossProduct > 0 ||
          (crossProduct == 0 &&
           ((point.x - prevPoint.x) * (point.x - prevPoint.x) + +(point.y - prevPoint.y) * (point.y - prevPoint.y)) >
               +((nextPoint.x - prevPoint.x) * (nextPoint.x - prevPoint.x) +
                 +(nextPoint.y - prevPoint.y) * (nextPoint.y - prevPoint.y)))) {
        nextPoint = point;
      }
    }

    if (nextPoint == p0) break;
    convexHull.push_back(nextPoint);
    prevPoint = nextPoint;
  }

  return convexHull;
}

std::vector<Point> Jarvis_omp(const std::vector<Point>& Points, int num_threads) {
  std::vector<Point> result;
  omp_set_num_threads(num_threads);

#pragma omp parallel
  {
    int threadNum = omp_get_thread_num();
    int localSize;
    int delta = Points.size() / num_threads;
    int remains = Points.size() % num_threads;
    std::vector<Point> localVector;

    if (threadNum == 0) {
      localSize = remains + delta;
      localVector.assign(Points.begin(), Points.begin() + localSize);
    } else {
      localSize = Points.size() / num_threads;
      localVector.assign(Points.begin() + (localSize * threadNum) + remains,
                         Points.begin() + (localSize * (threadNum + 1)) + remains);
    }
    std::vector<Point> localRes = Jarvis(localVector);
#pragma omp critical
    { result.insert(result.end(), localRes.begin(), localRes.end()); }
  }
  return Jarvis(result);
}

bool TestOMPJarvisSeq::pre_processing() {
  internal_order_test();
  points.resize(taskData->inputs_count[0]);
  auto* tmp_ptr = reinterpret_cast<Point*>(taskData->inputs[0]);
  std::copy_n(tmp_ptr, taskData->inputs_count[0], points.begin());
  return true;
}

bool TestOMPJarvisSeq::validation() {
  internal_order_test();
  if (taskData->inputs_count[0] == 0) {
    return false;
  }
  std::sort(points.begin(), points.end());
  return std::unique(points.begin(), points.end()) == points.end();
}

bool TestOMPJarvisSeq::run() {
  internal_order_test();
  resPoints = Jarvis(points);
  return true;
}

bool TestOMPJarvisSeq::post_processing() {
  internal_order_test();
  auto* output_ptr = reinterpret_cast<Point*>(taskData->outputs[0]);
  std::copy_n(resPoints.begin(), resPoints.size(), output_ptr);
  return true;
}

bool TestOMPJarvisParallel::pre_processing() {
  internal_order_test();
  points.resize(taskData->inputs_count[0]);
  auto* tmp_ptr = reinterpret_cast<Point*>(taskData->inputs[0]);
  std::copy_n(tmp_ptr, taskData->inputs_count[0], points.begin());
  return true;
}

bool TestOMPJarvisParallel::validation() {
  internal_order_test();
  if (taskData->inputs_count[0] == 0) {
    return false;
  }
  std::sort(points.begin(), points.end());
  return std::unique(points.begin(), points.end()) == points.end();
}

bool TestOMPJarvisParallel::run() {
  internal_order_test();
  resPoints = Jarvis_omp(points, 4);
  return true;
}

bool TestOMPJarvisParallel::post_processing() {
  internal_order_test();
  auto* output_ptr = reinterpret_cast<Point*>(taskData->outputs[0]);
  std::copy_n(resPoints.begin(), resPoints.size(), output_ptr);
  return true;
}
