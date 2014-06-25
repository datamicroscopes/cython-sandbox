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

  // make construction from python easier!
  mixturemodel_state(
      size_t n,
      const std::vector< std::string > &factories,
      const std::vector< hyperparam_t > &hyperparams)
    : gcount_(),
      assignments_(n, -1),
      hyperparams_(hyperparams)
  {
    assert(factories.size() == hyperparams.size());
    for (const auto &name : factories)
      factories_.emplace_back(component::metafactory(name));
  }

  mixturemodel_state(
      size_t n,
      const std::vector<std::function<std::shared_ptr<component>(const hyperparam_t &)>> &factories,
      const std::vector<hyperparam_t> &hyperparams)
    : gcount_(),
      assignments_(n, -1),
      factories_(factories),
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
    return gid;
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
    it->second.first++;
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
    it->second.first--;
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
    // XXX: missing the assignment model
    for (auto &group : groups_) {
      acc.reset();
      float sum = 0.;
      for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
        sum += group.second.second[i]->score_value(hyperparams_[i], acc);
      ret.first.push_back(group.first);
      ret.second.push_back(sum);
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
