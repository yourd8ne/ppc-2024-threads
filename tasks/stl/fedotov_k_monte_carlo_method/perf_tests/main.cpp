// Copyright 2024 Fedotov Kirill
#include <gtest/gtest.h>

#include <chrono>
#include <cmath>
#include <memory>
#include <vector>

#include "core/perf/include/perf.hpp"
#include "stl/fedotov_k_monte_carlo_method/include/ops_stl.hpp"

using namespace fedotov_stl;

TEST(fedotov_k_test, test_pipeline_run) {
  double result_of_integration = 1;
  double (*function1)(double, double) = func_x_plus_y;

  // Create data
  std::vector<double> first_integral_limit = {0, 1};
  std::vector<double> second_integral_limit = {0, 1};
  std::vector<double> output(1, result_of_integration);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_integral_limit.data()));
  taskDataSeq->inputs_count.emplace_back(first_integral_limit.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_integral_limit.data()));
  taskDataSeq->inputs_count.emplace_back(second_integral_limit.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(function1));
  taskDataSeq->inputs_count.emplace_back(second_integral_limit.size());

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  auto testTaskSequential = std::make_shared<FedotovTaskSeq>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->pipeline_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_NEAR(1, result_of_integration, 0.01);
}

TEST(fedotov_k_test, test_task_run) {
  double result_of_integration = 1;
  double (*function1)(double, double) = func_x_plus_y;

  // Create data
  std::vector<double> first_integral_limit = {0, 1};
  std::vector<double> second_integral_limit = {0, 1};
  std::vector<double> output(1, result_of_integration);

  // Create TaskData
  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(first_integral_limit.data()));
  taskDataSeq->inputs_count.emplace_back(first_integral_limit.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(second_integral_limit.data()));
  taskDataSeq->inputs_count.emplace_back(second_integral_limit.size());
  taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t *>(function1));
  taskDataSeq->inputs_count.emplace_back(second_integral_limit.size());

  taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t *>(output.data()));
  taskDataSeq->outputs_count.emplace_back(output.size());

  // Create Task
  auto testTaskSequential = std::make_shared<FedotovTaskSeq>(taskDataSeq);

  // Create Perf attributes
  auto perfAttr = std::make_shared<ppc::core::PerfAttr>();
  perfAttr->num_running = 10;
  const auto t0 = std::chrono::high_resolution_clock::now();
  perfAttr->current_timer = [&] {
    auto current_time_point = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time_point - t0).count();
    return static_cast<double>(duration) * 1e-9;
  };

  // Create and init perf results
  auto perfResults = std::make_shared<ppc::core::PerfResults>();

  // Create Perf analyzer
  auto perfAnalyzer = std::make_shared<ppc::core::Perf>(testTaskSequential);
  perfAnalyzer->task_run(perfAttr, perfResults);
  ppc::core::Perf::print_perf_statistic(perfResults);
  ASSERT_NEAR(1, result_of_integration, 0.01);
}