#include "mixturemodel.hpp"
#include "macros.hpp"
#include "util.hpp"

#include <iostream>

using namespace std;

size_t
mixturemodel_state::create_group(rng_t &rng)
{
  vector<shared_ptr<component>> gdata;
  gdata.reserve(factories_.size());
  for (size_t i = 0; i < factories_.size(); i++)
    gdata.emplace_back(factories_[i](*hyperparams_[i], rng));
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
mixturemodel_state::ensure_k_empty_groups(size_t k, rng_t &rng)
{
  // XXX: should allow for resampling
  if (emptygroups().size() == k)
    return;
  // XXX: NOT EFFICIENT
  vector<size_t> empty_groups(gempty_.begin(), gempty_.end());
  for (auto egid : empty_groups)
    remove_group(egid);
  for (size_t i = 0; i < k; i++)
    create_group(rng);
  assert( emptygroups().size() == k );
}

vector<runtime_type_info>
mixturemodel_state::get_runtime_type_info() const
{
  // XXX: move it somewhere else
  DCHECK(emptygroups().size(), "stupid implementation limitation");
  vector<runtime_type_info> ret;
  for (const auto &px : groups_.begin()->second.second)
    ret.push_back(px->get_runtime_type_info());
  return ret;
}

void
mixturemodel_state::add_value(size_t gid, const dataview &view, rng_t &rng)
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
  for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump()) {
    if (unlikely(acc.ismasked()))
      continue;
    it->second.second[i]->add_value(*hyperparams_[i], acc, rng);
  }
  assignments_[view.index()] = gid;
}

size_t
mixturemodel_state::remove_value(const dataview &view, rng_t &rng)
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
  for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump()) {
    if (unlikely(acc.ismasked()))
      continue;
    it->second.second[i]->remove_value(*hyperparams_[i], acc, rng);
  }
  assignments_[view.index()] = -1;
  return gid;
}

pair<vector<size_t>, vector<float>>
mixturemodel_state::score_value(row_accessor &acc, rng_t &rng) const
{
  pair<vector<size_t>, vector<float>> ret;
  const size_t n_empty_groups = gempty_.size();
  assert(n_empty_groups);
  const float empty_group_alpha = alpha_ / float(n_empty_groups);
  size_t count = 0;
  for (auto &group : groups_) {
    float sum = logf(group.second.first ? float(group.second.first) : empty_group_alpha);
    acc.reset();
    for (size_t i = 0; i < acc.nfeatures(); i++, acc.bump()) {
      if (unlikely(acc.ismasked()))
        continue;
      sum += group.second.second[i]->score_value(*hyperparams_[i], acc, rng);
    }
    ret.first.push_back(group.first);
    ret.second.push_back(sum);
    count += group.second.first;
  }
  const float lgnorm = logf(float(count) + alpha_);
  for (auto &s : ret.second)
    s -= lgnorm;
  return ret;
}

float
mixturemodel_state::score_data(const vector<size_t> &features,
                               const vector<size_t> &groups,
                               rng_t &rng) const
{
  // XXX: out of laziness, we copy
  vector<size_t> fids(features);
  if (fids.empty())
    util::inplace_range(fids, hyperparams_.size());
  vector<size_t> gids(groups);
  if (gids.empty()) {
    gids.reserve(groups_.size());
    for (auto &g : groups_)
      gids.push_back(g.first);
  }

  float sum = 0.;
  for (auto g : gids) {
    const auto &gdata = groups_.at(g);
    for (auto f : fids)
      sum += gdata.second[f]->score_data(*hyperparams_[f], rng);
  }
  return sum;
}

void
mixturemodel_state::sample_post_pred(row_accessor &acc,
                                     row_mutator &mut,
                                     rng_t &rng) const
{
  assert(acc.nfeatures() == mut.nfeatures());
  auto scores = score_value(acc, rng);
  const auto choice = scores.first[util::sample_discrete_log(scores.second, rng)];
  const auto &gdata = groups_.at(choice).second;
  acc.reset(); mut.reset();
  for (size_t i = 0; !acc.end(); acc.bump(), mut.bump(), i++) {
    if (!acc.ismasked()) {
      mut.set(acc);
      continue;
    }
    gdata[i]->sample_value(*hyperparams_[i], mut, rng);
  }
}
