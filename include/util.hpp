#pragma once

#include "dataview.hpp"
#include "mixturemodel.hpp"

struct util {
  static void
  stupid_bootstrap(mixturemodel_state &state, dataview &view)
  {
    size_t gid = state.create_group();
    for (view.reset(); !view.end(); view.next())
      state.add_value(gid, view);
  }
};
