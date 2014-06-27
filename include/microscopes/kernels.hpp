#pragma once

#include <microscopes/dataview.hpp>
#include <microscopes/mixturemodel.hpp>
#include <microscopes/random_fwd.hpp>

namespace microscopes {

struct bootstrap {
  static void
  likelihood(mixturemodel_state &state, dataview &view, rng_t &rng);
};

struct gibbs {
  static void
  assign(mixturemodel_state &state, dataview &view, rng_t &rng);
};

} // namespace microscopes
