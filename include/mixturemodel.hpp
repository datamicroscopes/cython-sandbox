#pragma once

#include "component.hpp"

#include <cassert>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <utility>

class mixturemodel_state {
public:
  mixturemodel_state(
      size_t n,
      const std::vector<std::function<std::shared_ptr<component>(const hyperparam_t &)>> &factories,
      const std::vector<hyperparam_t> &hyperparams)
    : gcount_(),
      assignments_(n, -1),
      factories_(factories)
      hyperparams_(hyperparams)
  {
    assert(factories.size() == hyperparams.size());
  }

  inline std::vector<size_t>
  emptygroups() const
  {
    std::vector<size_t> ret;
    for (auto &group : groups_)
      if (!group.second.first)
        ret.push_back(group.first);
    return ret;
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
    std::vector< std::shared_ptr<component> > gdata;
    for (size_t i = 0; i < factories_.size(); i++)
      gdata.emplace_back( factories_[i](hyperparams_[i]) );
    const size_t gid = gcount_++;
    groups_[gid] = std::make_pair(0, std::move(gdata));
    return gid
  }

  void
  remove_group(size_t gid)
  {
    auto it = groups_.find(gid);
    assert(it != groups_.end());
    assert(!it->second.first);
    groups_.erase(it);
  }

  void
  add_value(size_t gid, const dataview &view)
  {
    assert(view.size() == assignments_.size());
    assert(assignments_[view.index()] == -1);
    auto it = groups_.find(gid);
    assert(it != groups_.end());
    it->first++;
    row_accessor acc = view.get();
    assert(acc.nfeatures() == it->second.size());
    for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
      it->second[i].second->add_value(acc);
  }

  size_t
  remove_value(const dataview &view)
  {
    assert(view.size() == assignments_.size());
    assert(assignments_[view.index()] != -1);
    const size_t gid = assignments_[view.index()];
    auto it = groups_.find(gid);
    assert(it != groups_.end());
    it->first--;
    row_accessor acc = view.get();
    assert(acc.nfeatures() == it->second.size());
    for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
      it->second[i].second->remove_value(acc);
    return gid;
  }

  std::pair<std::vector<size_t>, std::vector<float>>
  score_value(row_accessor &acc) const
  {
    std::pair<std::vector<size_t>, std::vector<float>> ret;
    // XXX: missing the assignment model
    for (auto &group : groups_) {
      acc.reset();
      float acc = 0.;
      for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
        acc += group.second[i].second->score_value(acc);
      ret.first.push_back(group.first);
      ret.second.push_back(acc);
    }
    return ret;
  }

private:
  size_t gcount_;
  std::vector<ssize_t> assignments_;
  std::vector<std::function<std::shared_ptr<component>(const hyperparam_t &)>> factories_;
  std::vector<hyperparam_t> hyperparams_;
  std::map<size_t,
    std::pair<
      size_t,
      std::vector<std::shared_ptr<component>>>> groups_;
};
