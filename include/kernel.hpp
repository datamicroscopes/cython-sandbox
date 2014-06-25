#pragma once

#include "dataview.hpp"
#include "mixturemodel.hpp"
#include "util.hpp"
#include "macros.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

struct gibbs {

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
    const float acc = std::accumulate(scores.begin(), scores.end(), 0.);
    for (auto &s : scores)
      s /= acc;
  }

  static inline size_t
  sample_discrete_log(std::vector<float> &scores, rng_t &rng)
  {
    scores_to_probs(scores);
    return sample_discrete(scores, rng);
  }

  static inline size_t
  sample_discrete(const std::vector<float> &probs, rng_t &rng)
  {
    // assumes probs add up to 1
    float dart = Unif01_(rng);
    for (size_t i = 0; i < probs.size(); i++) {
      dart -= probs[i];
      if (dart <= 0.)
        return i;
    }
    return probs.size() - 1;
  }

  static void
  assign(mixturemodel_state &state, dataview &view, rng_t &rng)
  {
    // ensure 1 empty group
    size_t egid = 0;
    const size_t egsizeinit = state.emptygroups().size();
    if (!egsizeinit)
      egid = state.create_group(rng);
    else {
      auto it = state.emptygroups().begin();
      egid = *it++;
      if (egsizeinit > 1) {
        std::vector<size_t> egremove(it, state.emptygroups().end());
        for (auto g : egremove)
          state.remove_group(g);
      }
    }
    //std::cout << "empty group: " << egid << std::endl;
    for (view.reset(); !view.end(); view.next()) {
      const size_t gid = state.remove_value(view, rng);
      //std::cout << "datapoint " << view.index() << " belongs to group " << gid << std::endl;
      if (!state.groupsize(gid)) {
        //std::cout << "  * remove from group " << gid << std::endl;
        state.remove_group(gid);
      }
      row_accessor acc = view.get();
      auto scores = state.score_value(acc, rng);
      //std::cout << "  * scores: " << scores.second << std::endl;
      const auto choice = scores.first[sample_discrete_log(scores.second, rng)];
      //std::cout << "  * probs: " << scores.second << std::endl;
      //std::cout << "  * add to group " << choice << std::endl;
      state.add_value(choice, view, rng);
      if (choice == egid) {
        egid = state.create_group(rng);
        //std::cout << "  * new empty group: " << egid << std::endl;
      }
    }
    //std::cout << "currently " << state.ngroups() << std::endl;
  }

  static void
  bootstrap(mixturemodel_state &state, dataview &view, rng_t &rng)
  {
    DCHECK(!state.ngroups(), "not a clean state");
    view.reset();
    state.add_value(state.create_group(rng), view, rng);
    size_t egid = state.create_group(rng);
    for (view.next(); !view.end(); view.next()) {
      row_accessor acc = view.get();
      auto scores = state.score_value(acc, rng);
      //std::cout << "  * scores: " << scores.second << std::endl;
      const auto choice = scores.first[sample_discrete_log(scores.second, rng)];
      //std::cout << "  * probs: " << scores.second << std::endl;
      //std::cout << "  * add to group " << choice << std::endl;
      state.add_value(choice, view, rng);
      if (choice == egid)
        egid = state.create_group(rng);
    }
    //std::cout << "placed in " << state.ngroups() << std::endl;
  }

};
