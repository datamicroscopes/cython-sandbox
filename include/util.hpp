#pragma once

#include "dataview.hpp"
#include "mixturemodel.hpp"
#include "random_fwd.hpp"

#include <iostream>
#include <vector>
#include <utility>

// pretty printer for std::pair<A, B>
template <typename A, typename B>
static inline std::ostream &
operator<<(std::ostream &o, const std::pair<A, B> &p)
{
  o << "[" << p.first << ", " << p.second << "]";
  return o;
}

// pretty printer for std::vector<T, Alloc>
template <typename T, typename Alloc>
static inline std::ostream &
operator<<(std::ostream &o, const std::vector<T, Alloc> &v)
{
  bool first = true;
  o << "[";
  for (auto &p : v) {
    if (!first)
      o << ", ";
    first = false;
    o << p;
  }
  o << "]";
  return o;
}

struct util {

  static inline void
  inplace_range(std::vector<size_t> &a, size_t n)
  {
    a.clear();
    a.reserve(n);
    for (size_t i = 0; i < n; i++)
      a.push_back(i);
  }

  // generate a random permutation of the integers [0, ..., n-1]
  static inline void
  permute(std::vector<size_t> &pi, size_t n, rng_t &rng)
  {
    inplace_range(pi, n);
    for (size_t i = pi.size() - 1; i >= 1; i--) {
      std::uniform_int_distribution<> dist(0, i);
      const size_t j = dist(rng);
      std::swap(pi[j], pi[i]);
    }
  }

};
