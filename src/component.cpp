#include "component.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>

using namespace std;

function<shared_ptr<component>(const hyperparam_t &)>
component::metafactory(const string &name)
{
  if (name == "beta_bernoulli")
    return [](const hyperparam_t &hps) {
      return std::make_shared<beta_bernoulli>(hps);
    };
  throw runtime_error("unknown component name");
}

void
beta_bernoulli::add_value(const row_accessor &value)
{
  if (value.get<bool>())
    heads_++;
  else
    tails_++;
}

void
beta_bernoulli::remove_value(const row_accessor &value)
{
  if (value.get<bool>()) {
    assert(heads_);
    heads_--;
  } else {
    assert(tails_);
    tails_--;
  }
}

float
beta_bernoulli::score_value(const row_accessor &value) const
{
  assert(*alpha_ > 0.0);
  assert(*beta_ > 0.0);
  const float effective_heads = *alpha_ + float(heads_);
  const float effective_tails = *beta_ + float(tails_);
  const float numer = value.get<bool>() ? effective_heads : effective_tails;
  const float denom = effective_heads + effective_tails;
  return logf(numer / denom);
}
