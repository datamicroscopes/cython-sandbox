#pragma once

#include "component.hpp"
#include "macros.hpp"

#include <cassert>
#include <cmath>
#include <vector>
#include <set>
#include <functional>
#include <map>
#include <memory>
#include <utility>

class mixturemodel_state {
public:

  // make construction from python easier!
  mixturemodel_state(
      size_t n,
      const hyperparam_bag_t &clusterhp,
      const std::vector< std::string > &factorynames,
      const std::vector< hyperparam_bag_t > &bags)
    : alpha_(clusterhp.at("alpha")),
      gcount_(),
      gremoved_(),
      assignments_(n, -1)
  {
    DCHECK(factorynames.size() == bags.size(), "size discrepancy");
    for (size_t i = 0; i < factorynames.size(); i++) {
      const auto &name = factorynames[i];
      const auto fns = component::metafactory(name);
      hyperparams_.emplace_back(fns.first(bags[i]));
      factories_.emplace_back(fns.second);
    }
  }

  inline const std::set<size_t> &
  emptygroups() const
  {
    return gempty_;
  }

  inline size_t
  groupsize(size_t gid) const
  {
    const auto it = groups_.find(gid);
    assert(it != groups_.end());
    return it->second.first;
  }

  inline size_t ngroups() const { return groups_.size(); }
  inline size_t nentities() const { return assignments_.size(); }

  size_t create_group(rng_t &rng);
  void remove_group(size_t gid);

  void add_value(size_t gid, const dataview &view, rng_t &rng);
  size_t remove_value(const dataview &view, rng_t &rng);
  std::pair<std::vector<size_t>, std::vector<float>> score_value(row_accessor &acc, rng_t &rng) const;
  float score_data(const std::vector<size_t> &features, const std::vector<size_t> &groups, rng_t &rng) const;

  // random statistics
  inline size_t groups_created() const { return gcount_; }
  inline size_t groups_removed() const { return gremoved_; }

private:
  float alpha_;
  size_t gcount_;
  size_t gremoved_;
  std::set<size_t> gempty_;
  std::vector<ssize_t> assignments_;
  std::vector<std::shared_ptr<hyperparam>> hyperparams_;
  std::vector<component::make_component_fn_t> factories_;
  std::map<size_t,
    std::pair<
      size_t,
      std::vector<std::shared_ptr<component>>>> groups_;
};
