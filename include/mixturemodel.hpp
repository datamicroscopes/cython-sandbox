#pragma once

#include "component.hpp"

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
      assignments_(n, -1),
      hyperparams_(hyperparams)
  {
    assert(factories.size() == hyperparams.size());
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
      assignments_(n, -1),
      factories_(factories),
      hyperparams_(hyperparams)
  {
    assert(factories.size() == hyperparams.size());
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

  size_t
  create_group()
  {
    std::vector<std::shared_ptr<component>> gdata;
    gdata.reserve(factories_.size());
    for (size_t i = 0; i < factories_.size(); i++)
      gdata.emplace_back(factories_[i](hyperparams_[i]));
    const size_t gid = gcount_++;
    groups_[gid] = std::move(std::make_pair(0, std::move(gdata)));
    assert(!gempty_.count(gid));
    gempty_.insert(gid);
    return gid;
  }

  void
  remove_group(size_t gid)
  {
    auto it = groups_.find(gid);
    assert(it != groups_.end());
    assert(!it->second.first);
    assert(gempty_.count(gid));
    groups_.erase(it);
    gempty_.erase(gid);
  }

  void
  add_value(size_t gid, const dataview &view)
  {
    assert(view.size() == assignments_.size());
    assert(assignments_[view.index()] == -1);
    auto it = groups_.find(gid);
    assert(it != groups_.end());
    if (!it->second.first++) {
      assert(gempty_.count(gid));
      gempty_.erase(gid);
    } else {
      assert(!gempty_.count(gid));
    }
    row_accessor acc = view.get();
    assert(acc.nfeatures() == it->second.second.size());
    for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
      it->second.second[i]->add_value(acc);
    assignments_[view.index()] = gid;
  }

  size_t
  remove_value(const dataview &view)
  {
    assert(view.size() == assignments_.size());
    assert(assignments_[view.index()] != -1);
    const size_t gid = assignments_[view.index()];
    auto it = groups_.find(gid);
    assert(it != groups_.end());
    assert(!gempty_.count(gid));
    if (!--it->second.first)
      gempty_.insert(gid);
    row_accessor acc = view.get();
    assert(acc.nfeatures() == it->second.second.size());
    for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
      it->second.second[i]->remove_value(acc);
    assignments_[view.index()] = -1;
    return gid;
  }

  std::pair<std::vector<size_t>, std::vector<float>>
  score_value(row_accessor &acc) const
  {
    std::pair<std::vector<size_t>, std::vector<float>> ret;
    const size_t n_empty_groups = gempty_.size();
    assert(n_empty_groups);
    const float empty_group_alpha = alpha_ / float(n_empty_groups);
    size_t count = 0;
    for (auto &group : groups_) {
      float sum = logf(group.second.first ? float(group.second.first) : empty_group_alpha);
      acc.reset();
      for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
        sum += group.second.second[i]->score_value(hyperparams_[i], acc);
      ret.first.push_back(group.first);
      ret.second.push_back(sum);
      count += group.second.first;
    }
    const float lgnorm = logf(float(count) + alpha_);
    for (auto &s : ret.second)
      s -= lgnorm;
    return ret;
  }

private:
  float alpha_;
  size_t gcount_;
  std::set<size_t> gempty_;
  std::vector<ssize_t> assignments_;
  std::vector<std::function<std::shared_ptr<component>(const hyperparam_t &)>> factories_;
  std::vector<hyperparam_t> hyperparams_;
  std::map<size_t,
    std::pair<
      size_t,
      std::vector<std::shared_ptr<component>>>> groups_;
};
