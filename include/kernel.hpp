#pragma once

#include "dataview.hpp"
#include "mixturemodel.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

struct gibbs {

  // XXX: BAD! NEED TO THREAD RANDOMNESS THROUGH THE PROGRAM
  static std::default_random_engine Prng_;
  static std::uniform_real_distribution<float> Unif01_;

  // copy from:
  // https://github.com/forcedotcom/distributions/blob/master/distributions/util.py

  static inline void
  scores_to_probs(std::vector<float> &scores)
  {
    const float m = *std::max_element(scores.begin(), scores.end());
    for (auto &s : scores)
      s -= m;
    for (auto &s : scores)
      s = expf(s);
    const float acc = std::accumulate(scores.begin(), scores.end(), 0);
    for (auto &s : scores)
      s /= acc;
  }

  static inline size_t
  sample_discrete_log(std::vector<float> &scores)
  {
    scores_to_probs(scores);
    return sample_discrete(scores);
  }

  static inline size_t
  sample_discrete(const std::vector<float> &probs)
  {
    // assumes probs add up to 1
    float dart = Unif01_(Prng_);
    for (size_t i = 0; i < probs.size(); i++) {
      dart -= probs[i];
      if (dart <= 0.)
        return i;
    }
    return probs.size() - 1;
  }

  static void
  assign(mixturemodel_state &state, dataview &view)
  {
    // ensure 1 empty group
    // XXX: lazy
    for (auto gid : state.emptygroups())
      state.remove_group(gid);
    size_t egid = state.create_group();
    for (view.reset(); !view.end(); view.next()) {
      const size_t gid = state.remove_value(view);
      if (!state.groupsize(gid))
        state.remove_group(gid);
      row_accessor acc = view.get();
      auto scores = state.score_value(acc);
      const auto choice = scores.first[sample_discrete_log(scores.second)];
      state.add_value(choice, view);
      if (choice == egid)
        egid = state.create_group();
      std::cout << "finished datapoint " << view.index() << std::endl;
      std::cout << "nempty groups " << state.emptygroups().size() << std::endl;
    }
  }

};
