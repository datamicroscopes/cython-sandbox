#include "kernel.hpp"
#include "macros.hpp"
#include "util.hpp"

#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iostream>

using namespace std;

void
gibbs::assign(mixturemodel_state &state, dataview &view, rng_t &rng)
{
  // ensure 1 empty group
  // note: this is more efficient than state.ensure_k_empty_groups(1, rng)
  size_t egid = 0;
  const size_t egsizeinit = state.emptygroups().size();
  if (!egsizeinit)
    egid = state.create_group(rng);
  else {
    auto it = state.emptygroups().begin();
    egid = *it++;
    if (egsizeinit > 1) {
      vector<size_t> egremove(it, state.emptygroups().end());
      for (auto g : egremove)
        state.remove_group(g);
    }
  }

  //cout << "empty group: " << egid << endl;
  for (view.reset(); !view.end(); view.next()) {
    const size_t gid = state.remove_value(view, rng);
    //cout << "datapoint " << view.index() << " belongs to group " << gid << endl;
    if (!state.groupsize(gid)) {
      //cout << "  * remove from group " << gid << endl;
      state.remove_group(gid);
    }
    row_accessor acc = view.get();
    auto scores = state.score_value(acc, rng);
    //cout << "  * scores: " << scores.second << endl;
    const auto choice = scores.first[util::sample_discrete_log(scores.second, rng)];
    //cout << "  * probs: " << scores.second << endl;
    //cout << "  * add to group " << choice << endl;
    state.add_value(choice, view, rng);
    if (choice == egid) {
      egid = state.create_group(rng);
      //cout << "  * new empty group: " << egid << endl;
    }
  }
  //cout << "currently " << state.ngroups() << endl;
}

void
gibbs::bootstrap(mixturemodel_state &state, dataview &view, rng_t &rng)
{
  DCHECK(!state.ngroups(), "not a clean state");
  view.reset();
  state.add_value(state.create_group(rng), view, rng);
  size_t egid = state.create_group(rng);
  for (view.next(); !view.end(); view.next()) {
    row_accessor acc = view.get();
    auto scores = state.score_value(acc, rng);
    //cout << "  * scores: " << scores.second << endl;
    const auto choice = scores.first[util::sample_discrete_log(scores.second, rng)];
    //cout << "  * probs: " << scores.second << endl;
    //cout << "  * add to group " << choice << endl;
    state.add_value(choice, view, rng);
    if (choice == egid)
      egid = state.create_group(rng);
  }
  //cout << "placed in " << state.ngroups() << endl;
}
