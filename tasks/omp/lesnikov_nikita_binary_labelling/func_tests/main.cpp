// Copyright 2024 Lesnikov Nikita
#include <gtest/gtest.h>

#include <vector>

#include "omp/lesnikov_nikita_binary_labelling/include/ops_omp.hpp"

TEST(Parallel_Operations_OpenMP, Test_1_bin_lab) {
  int m = 1000;
  int n = 1000;
  auto serializedM = serializeInt32(m);
  auto serializedN = serializeInt32(n);
  std::vector<uint8_t> in = getRandomVector(m * n);

  std::vector<uint8_t> outV_seq(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();
  taskDataSeq->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataSeq->inputs.push_back(in.data());
  taskDataSeq->inputs.push_back(serializedM.data());
  taskDataSeq->inputs.push_back(serializedN.data());
  taskDataSeq->inputs_count.push_back(in.size());
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->inputs_count.push_back(4);
  taskDataSeq->outputs.push_back(outV_seq.data());
  taskDataSeq->outputs_count.push_back(outV_seq.size());

  BinaryLabellingSeq testTaskSequential(taskDataSeq);
  ASSERT_TRUE(testTaskSequential.validation());
  ASSERT_TRUE(testTaskSequential.pre_processing());
  ASSERT_TRUE(testTaskSequential.run());
  ASSERT_TRUE(testTaskSequential.post_processing());

  std::vector<int> outVD_seq = deserializeInt32V(outV_seq);

  std::vector<uint8_t> outV_omp(in.size() * sizeof(int));

  std::shared_ptr<ppc::core::TaskData> taskDataOmp = std::make_shared<ppc::core::TaskData>();
  taskDataOmp->state_of_testing = ppc::core::TaskData::FUNC;
  taskDataOmp->inputs.push_back(in.data());
  taskDataOmp->inputs.push_back(serializedM.data());
  taskDataOmp->inputs.push_back(serializedN.data());
  taskDataOmp->inputs_count.push_back(in.size());
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->inputs_count.push_back(4);
  taskDataOmp->outputs.push_back(outV_omp.data());
  taskDataOmp->outputs_count.push_back(outV_omp.size());

  BinaryLabellingOmp testTaskOmp(taskDataOmp);
  ASSERT_TRUE(testTaskOmp.validation());
  ASSERT_TRUE(testTaskOmp.pre_processing());
  ASSERT_TRUE(testTaskOmp.run());
  ASSERT_TRUE(testTaskOmp.post_processing());

  std::vector<int> outVD_omp = deserializeInt32V(outV_omp);

  EXPECT_EQ(getObjectsNum(outVD_omp), getObjectsNum(outVD_seq));
  EXPECT_TRUE(isMapsEqual(outVD_omp, outVD_seq));
}
