#pragma once

#include "dataview.hpp"
#include <cmath>
#include <map>

// XXX: need to generalize a bag of floats
typedef std::map<std::string, float> hyperparam_t;

// abstract component model
class component {
public:
  virtual ~component() {}
  virtual void add_value(const row_accessor &value) = 0;
  virtual void remove_value(const row_accessor &value) = 0;
  virtual float score_value(const hyperparam_t &hps, const row_accessor &value) const = 0;
};

class beta_bernoulli : public component {
public:
  beta_bernoulli() : heads_(), tails_() {}

  void
  add_value(const row_accessor &value) override
  {
    if (value.get<bool>())
      heads_++;
    else
      tails_++;
  }

  void
  remove_value(const row_accessor &value) override
  {
    if (value.get<bool>())
      heads_--;
    else
      tails_--;
  }

  float
  score_value(const hyperparam_t &hps, const row_accessor &value) const override
  {
    const float alpha = hps["alpha"];
    const float beta  = hps["beta"];
    return betaln(alpha + float(heads_), beta + float(tails_)) - betaln(alpha, beta);
  }

private:
  static inline float
  betaln(float x, float y)
  {
    return lgammaf(x) + lgammaf(y) - lgammaf(x + y);
  }

  size_t heads_;
  size_t tails_;
};
