//=== llvm/unittest/ADT/EquivalenceClassesTest.cpp - the structure tests --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/EquivalenceClasses.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace llvm;

namespace llvm {

TEST(EquivalenceClassesTest, CopyAssignemnt) {
  EquivalenceClasses<int> EC, Copy;
  EC.insert(1);
  EC.insert(4);
  EquivalenceClasses<int> &Ref = Copy = EC;
  EXPECT_EQ(Copy.getNumClasses(), 2u);
  EXPECT_EQ(&Ref, &Copy);
}

TEST(EquivalenceClassesTest, NoMerges) {
  EquivalenceClasses<int> EqClasses;
  // Until we merged any sets, check that every element is only equivalent to
  // itself.
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (i == j)
        EXPECT_TRUE(EqClasses.isEquivalent(i, j));
      else
        EXPECT_FALSE(EqClasses.isEquivalent(i, j));
}

TEST(EquivalenceClassesTest, SimpleMerge1) {
  EquivalenceClasses<int> EqClasses;
  // Check that once we merge (A, B), (B, C), (C, D), then all elements belong
  // to one set.
  EqClasses.unionSets(0, 1);
  EqClasses.unionSets(1, 2);
  EqClasses.unionSets(2, 3);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_TRUE(EqClasses.isEquivalent(i, j));
}

TEST(EquivalenceClassesTest, SimpleMerge2) {
  EquivalenceClasses<int> EqClasses;
  // Check that once we merge (A, B), (C, D), (A, C), then all elements belong
  // to one set.
  EqClasses.unionSets(0, 1);
  EqClasses.unionSets(2, 3);
  EqClasses.unionSets(0, 2);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_TRUE(EqClasses.isEquivalent(i, j));
}

TEST(EquivalenceClassesTest, SimpleErase1) {
  EquivalenceClasses<int> EqClasses;
  // Check that erase head success.
  // After erase A from (A, B ,C, D), <B, C, D> belong to one set.
  EqClasses.unionSets(0, 1);
  EqClasses.unionSets(2, 3);
  EqClasses.unionSets(0, 2);
  EXPECT_TRUE(EqClasses.erase(0));
  for (int i = 1; i < 4; ++i)
    for (int j = 1; j < 4; ++j)
      EXPECT_TRUE(EqClasses.isEquivalent(i, j));
}

TEST(EquivalenceClassesTest, SimpleErase2) {
  EquivalenceClasses<int> EqClasses;
  // Check that erase tail success.
  // After erase D from (A, B ,C, D), <A, B, C> belong to one set.
  EqClasses.unionSets(0, 1);
  EqClasses.unionSets(2, 3);
  EqClasses.unionSets(0, 2);
  EXPECT_TRUE(EqClasses.erase(3));
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_TRUE(EqClasses.isEquivalent(i, j));
}

TEST(EquivalenceClassesTest, SimpleErase3) {
  EquivalenceClasses<int> EqClasses;
  // Check that erase a value in the middle success.
  // After erase B from (A, B ,C, D), <A, C, D> belong to one set.
  EqClasses.unionSets(0, 1);
  EqClasses.unionSets(2, 3);
  EqClasses.unionSets(0, 2);
  EXPECT_TRUE(EqClasses.erase(1));
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_TRUE(EqClasses.isEquivalent(i, j) ^ ((i == 1) ^ (j == 1)));
}

TEST(EquivalenceClassesTest, SimpleErase4) {
  EquivalenceClasses<int> EqClasses;
  // Check that erase a single class success.
  EqClasses.insert(0);
  EXPECT_TRUE(EqClasses.getNumClasses() == 1);
  EXPECT_TRUE(EqClasses.erase(0));
  EXPECT_TRUE(EqClasses.getNumClasses() == 0);
  EXPECT_FALSE(EqClasses.erase(1));
}

TEST(EquivalenceClassesTest, TwoSets) {
  EquivalenceClasses<int> EqClasses;
  // Form sets of odd and even numbers, check that we split them into these
  // two sets correcrly.
  for (int i = 0; i < 30; i += 2)
    EqClasses.unionSets(0, i);
  for (int i = 1; i < 30; i += 2)
    EqClasses.unionSets(1, i);

  for (int i = 0; i < 30; i++)
    for (int j = 0; j < 30; j++)
      if (i % 2 == j % 2)
        EXPECT_TRUE(EqClasses.isEquivalent(i, j));
      else
        EXPECT_FALSE(EqClasses.isEquivalent(i, j));
}

TEST(EquivalenceClassesTest, MembersIterator) {
  EquivalenceClasses<int> EC;
  EC.unionSets(1, 2);
  EC.insert(4);
  EC.insert(5);
  EC.unionSets(5, 1);
  EXPECT_EQ(EC.getNumClasses(), 2u);

  EXPECT_THAT(EC.members(4), testing::ElementsAre(4));
  EXPECT_THAT(EC.members(1), testing::ElementsAre(5, 1, 2));
}

// Type-parameterized tests: Run the same test cases with different element
// types.
template <typename T> class ParameterizedTest : public testing::Test {};

TYPED_TEST_SUITE_P(ParameterizedTest);

TYPED_TEST_P(ParameterizedTest, MultipleSets) {
  TypeParam EqClasses;
  // Split numbers from [0, 100) into sets so that values in the same set have
  // equal remainders (mod 17).
  for (int i = 0; i < 100; i++)
    EqClasses.unionSets(i % 17, i);

  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      if (i % 17 == j % 17)
        EXPECT_TRUE(EqClasses.isEquivalent(i, j));
      else
        EXPECT_FALSE(EqClasses.isEquivalent(i, j));
}

} // llvm
