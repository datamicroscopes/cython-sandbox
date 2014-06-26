#pragma once

#include "dataview.hpp"
#include "mixturemodel.hpp"
#include "random_fwd.hpp"

struct gibbs {
  static void
  assign(mixturemodel_state &state, dataview &view, rng_t &rng);
  static void
  bootstrap(mixturemodel_state &state, dataview &view, rng_t &rng);
};
