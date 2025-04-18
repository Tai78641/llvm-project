//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <flat_set>

// [[nodiscard]] bool empty() const noexcept;

#include <flat_set>
#include <cassert>
#include <deque>
#include <functional>
#include <utility>
#include <vector>

#include "MinSequenceContainer.h"
#include "test_macros.h"
#include "min_allocator.h"

template <class KeyContainer>
void test_one() {
  using Key = typename KeyContainer::value_type;
  using M   = std::flat_multiset<Key, std::less<int>, KeyContainer>;
  M m;
  ASSERT_SAME_TYPE(decltype(m.empty()), bool);
  ASSERT_NOEXCEPT(m.empty());
  assert(m.empty());
  assert(std::as_const(m).empty());
  m = {1};
  assert(!m.empty());
  m.clear();
  assert(m.empty());
}

void test() {
  test_one<std::vector<int>>();
  test_one<std::deque<int>>();
  test_one<MinSequenceContainer<int>>();
  test_one<std::vector<int, min_allocator<int>>>();
}

int main(int, char**) {
  test();

  return 0;
}
