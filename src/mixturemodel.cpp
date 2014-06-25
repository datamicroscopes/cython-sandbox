#include "mixturemodel.hpp"
#include <iostream>

using namespace std;

size_t
mixturemodel_state::create_group()
{
  vector<shared_ptr<component>> gdata;
  gdata.reserve(factories_.size());
  for (size_t i = 0; i < factories_.size(); i++)
    gdata.emplace_back(factories_[i](hyperparams_[i]));
  const size_t gid = gcount_++;
  groups_[gid] = move(make_pair(0, move(gdata)));
  assert(!gempty_.count(gid));
  gempty_.insert(gid);
  return gid;
}

void
mixturemodel_state::remove_group(size_t gid)
{
  auto it = groups_.find(gid);
  assert(it != groups_.end());
  assert(!it->second.first);
  assert(gempty_.count(gid));
  groups_.erase(it);
  gempty_.erase(gid);
  gremoved_++;
}

void
mixturemodel_state::add_value(size_t gid, const dataview &view)
{
  assert(view.size() == assignments_.size());
  assert(assignments_.at(view.index()) == -1);
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
mixturemodel_state::remove_value(const dataview &view)
{
  assert(view.size() == assignments_.size());
  assert(assignments_.at(view.index()) != -1);
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

pair<vector<size_t>, vector<float>>
mixturemodel_state::score_value(row_accessor &acc) const
{
  pair<vector<size_t>, vector<float>> ret;
  const size_t n_empty_groups = gempty_.size();
  assert(n_empty_groups);
  const float empty_group_alpha = alpha_ / float(n_empty_groups);
  size_t count = 0;
  for (auto &group : groups_) {
    float sum = logf(group.second.first ? float(group.second.first) : empty_group_alpha);
    acc.reset();
    for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump())
      sum += group.second.second[i]->score_value(acc);
    ret.first.push_back(group.first);
    ret.second.push_back(sum);
    count += group.second.first;
  }
  const float lgnorm = logf(float(count) + alpha_);
  for (auto &s : ret.second)
    s -= lgnorm;
  return ret;
}
