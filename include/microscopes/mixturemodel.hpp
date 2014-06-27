#pragma once

#include <microscopes/macros.hpp>
#include <microscopes/models.hpp>

#include <cassert>
#include <cmath>
#include <vector>
#include <set>
#include <functional>
#include <map>
#include <memory>
#include <utility>

namespace microscopes {

class mixturemodel_state {
public:

  mixturemodel_state(size_t n, const std::vector<std::shared_ptr<model>> &models)
    : alpha_(),
      gcount_(),
      gremoved_(),
      gempty_(),
      assignments_(n, -1),
      models_(models),
      groups_()
  {
  }

  hyperparam_bag_t
  get_hp() const
  {
    // XXX: implement me
    return "";
  }

  void
  set_hp(const hyperparam_bag_t &hp)
  {
    // XXX: implement me
  }

  hyperparam_bag_t
  get_feature_hp(size_t i) const
  {
    return models_[i]->get_hp();
  }

  void
  set_feature_hp(size_t i, const hyperparam_bag_t &hp)
  {
    models_[i]->set_hp(hp);
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

  // XXX: helper function, move to outer mixturemodel once we
  // abstract better
  void ensure_k_empty_groups(size_t k, rng_t &rng);

  // XXX: also doesn't belong here
  std::vector< runtime_type_info >
  get_runtime_type_info() const;

  void add_value(size_t gid, const dataview &view, rng_t &rng);
  size_t remove_value(const dataview &view, rng_t &rng);
  std::pair<std::vector<size_t>, std::vector<float>> score_value(row_accessor &acc, rng_t &rng) const;
  float score_data(const std::vector<size_t> &features, const std::vector<size_t> &groups, rng_t &rng) const;

  // XXX: we assume the caller has taken care to set the groups correctly!
  void sample_post_pred(row_accessor &acc, row_mutator &mut, rng_t &rng) const;

  // random statistics
  inline size_t groups_created() const { return gcount_; }
  inline size_t groups_removed() const { return gremoved_; }

private:
  float alpha_;
  size_t gcount_;
  size_t gremoved_;
  std::set<size_t> gempty_;
  std::vector<ssize_t> assignments_;
  std::vector<std::shared_ptr<model>> models_;
  std::map<size_t, std::pair<size_t, std::vector<std::shared_ptr<feature_group>>>> groups_;
};

} // namespace microscopes
