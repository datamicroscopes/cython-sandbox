#pragma once

#include "dataview.hpp"
#include "mixturemodel.hpp"

struct gibbs {

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
      const auto scores = state.score_value(view.get());
      // XXX: sample
      const auto choice = scores.first[0];
      state.add_value(choice, view);
      if (choice == egid)
        egid = state.create_group();
    }
  }

};
