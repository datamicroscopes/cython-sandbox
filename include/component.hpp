#pragma once

#include "dataview.hpp"
#include <cmath>
#include <map>
#include <functional>
#include <memory>
#include <string>

// XXX: need to generalize a bag of floats
typedef std::map<std::string, float> hyperparam_t;

// abstract component model
class component {
public:
  virtual ~component() {}
  virtual void add_value(const row_accessor &value) = 0;
  virtual void remove_value(const row_accessor &value) = 0;
  virtual float score_value(const row_accessor &value) const = 0;

  // ugly hack for python
  static std::function<std::shared_ptr<component>(const hyperparam_t &)>
  metafactory(const std::string &name);
};

class beta_bernoulli : public component {
public:

  // XXX: assumes hps is stable memory
  beta_bernoulli(const hyperparam_t &hps)
    : alpha_(&hps.at("alpha")),
      beta_(&hps.at("beta")),
      heads_(), tails_() {}

  void add_value(const row_accessor &value) override;
  void remove_value(const row_accessor &value) override;
  float score_value(const row_accessor &value) const override;

private:
  static inline float
  betaln(float x, float y)
  {
    return lgammaf(x) + lgammaf(y) - lgammaf(x + y);
  }

  const float *alpha_;
  const float *beta_;
  size_t heads_;
  size_t tails_;
};
