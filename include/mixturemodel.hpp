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
      const hyperparam_t &clusterhp,
      const std::vector< std::string > &factories,
      const std::vector< hyperparam_t > &hyperparams)
    : alpha_(clusterhp.at("alpha")),
      gcount_(),
      gremoved_(),
      assignments_(n, -1),
      hyperparams_(hyperparams)
  {
    DCHECK(factories.size() == hyperparams.size(), "factories/hps need to be same size");
    for (const auto &name : factories)
      factories_.emplace_back(component::metafactory(name));
  }

  mixturemodel_state(
      size_t n,
      const hyperparam_t &clusterhp,
      const std::vector<std::function<std::shared_ptr<component>(const hyperparam_t &)>> &factories,
      const std::vector<hyperparam_t> &hyperparams)
    : alpha_(clusterhp.at("alpha")),
      gcount_(),
      gremoved_(),
      assignments_(n, -1),
      factories_(factories),
      hyperparams_(hyperparams)
  {
    DCHECK(factories.size() == hyperparams.size(), "factories/hps need to be same size");
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

  size_t create_group();
  void remove_group(size_t gid);

  void add_value(size_t gid, const dataview &view);
  size_t remove_value(const dataview &view);
  std::pair<std::vector<size_t>, std::vector<float>> score_value(row_accessor &acc) const;

  // random statistics
  inline size_t groups_created() const { return gcount_; }
  inline size_t groups_removed() const { return gremoved_; }

private:
  float alpha_;
  size_t gcount_;
  size_t gremoved_;
  std::set<size_t> gempty_;
  std::vector<ssize_t> assignments_;
  std::vector<std::function<std::shared_ptr<component>(const hyperparam_t &)>> factories_;
  std::vector<hyperparam_t> hyperparams_;
  std::map<size_t,
    std::pair<
      size_t,
      std::vector<std::shared_ptr<component>>>> groups_;
};
