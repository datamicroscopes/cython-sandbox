#pragma once

#include "dataview.hpp"
#include "random_fwd.hpp"
#include "type_helper.hpp"

#include <map>
#include <functional>
#include <memory>
#include <string>

// XXX: need to generalize a bag of floats
typedef std::map<std::string, float> hyperparam_bag_t;

// abstract hyperparameter class
class hyperparam {
public:
  virtual ~hyperparam() {}
};

// abstract component model
class component {
public:
  virtual ~component() {}
  virtual void add_value(const hyperparam &hp, const row_accessor &value, rng_t &rng) = 0;
  virtual void remove_value(const hyperparam &hp, const row_accessor &value, rng_t &rng) = 0;
  virtual float score_value(const hyperparam &hp, const row_accessor &value, rng_t &rng) const = 0;
  virtual float score_data(const hyperparam &hp, rng_t &rng) const = 0;
  virtual void sample_value(const hyperparam &hp, row_mutator &value, rng_t &rng) const = 0;

  virtual runtime_type_info get_runtime_type_info() const = 0;

  // ugly hack for python
  typedef std::function<std::shared_ptr<hyperparam>(const hyperparam_bag_t &)> make_hyperparam_fn_t;
  typedef std::function<std::shared_ptr<component>(const hyperparam &, rng_t &)> make_component_fn_t;
  static std::pair< make_hyperparam_fn_t, make_component_fn_t >
  metafactory(const std::string &name);
};

template <typename T, typename Message>
struct distributions_hyperparam : public hyperparam {

  distributions_hyperparam(const Message &m)
    : repr_()
  {
    repr_.protobuf_load(m);
  }

  typename T::Shared repr_;
};

// wrapper for distributions API
template <typename T, typename Message>
class distributions_component : public component {
public:

  distributions_component(const distributions_hyperparam<T, Message> &hp, rng_t &rng)
    : repr_()
  {
    repr_.init(hp.repr_, rng);
  }

  void
  add_value(const hyperparam &hp, const row_accessor &value, rng_t &rng) override
  {
    repr_.add_value(
      static_cast<const distributions_hyperparam<T, Message> &>(hp).repr_,
      value.get< typename T::Value >(),
      rng);
  }

  void
  remove_value(const hyperparam &hp, const row_accessor &value, rng_t &rng) override
  {
    repr_.remove_value(
      static_cast<const distributions_hyperparam<T, Message> &>(hp).repr_,
      value.get< typename T::Value >(),
      rng);
  }

  float
  score_value(const hyperparam &hp, const row_accessor &value, rng_t &rng) const override
  {
    return repr_.score_value(
      static_cast<const distributions_hyperparam<T, Message> &>(hp).repr_,
      value.get< typename T::Value >(),
      rng);
  }

  float
  score_data(const hyperparam &hp, rng_t &rng) const override
  {
    return repr_.score_data(
      static_cast<const distributions_hyperparam<T, Message> &>(hp).repr_,
      rng);
  }

  void
  sample_value(const hyperparam &hp, row_mutator &value, rng_t &rng) const override
  {
    typename T::Value sampled =
      repr_.sample_value(
          static_cast<const distributions_hyperparam<T, Message> &>(hp).repr_,
          rng);
    value.set< typename T::Value >(sampled);
  }

  runtime_type_info
  get_runtime_type_info() const override
  {
    return _static_type_to_runtime_id< typename T::Value >::value;
  }

private:
  typename T::Group repr_;
};

// This only works for distributions with hyperparameters that are
// all floats unfortunately

#define DISTRIB_GENERATE_FIELDS_ACTION(fname) float fname ## _;
#define DISTRIB_GENERATE_GETTERS_ACTION(fname) \
  inline float fname () const { return fname ## _; }
#define DISTRIB_GENERATE_SETTERS_ACTION(fname) \
  inline void set_ ## fname (float fname) { fname ## _ = fname; }
#define DISTRIB_GENERATE_LOAD_ACTION(fname) \
  ret.set_ ## fname(bag.at( #fname ));
#define DISTRIB_GENERATE_MESSAGE_CLASS(name, fields) \
  struct distributions_ ## name ## _message { \
    fields(DISTRIB_GENERATE_FIELDS_ACTION) \
    fields(DISTRIB_GENERATE_GETTERS_ACTION) \
    fields(DISTRIB_GENERATE_SETTERS_ACTION) \
    static distributions_ ## name ## _message \
    from_bag(const hyperparam_bag_t &bag) \
    { \
      distributions_ ## name ## _message ret; \
      fields(DISTRIB_GENERATE_LOAD_ACTION) \
      return ret; \
    } \
  };
#define DISTRIB_TYPEDEFS(clsname) \
  typedef \
    distributions_hyperparam< distributions::clsname, distributions_ ## clsname ## _message > \
    distributions_ ## clsname ## _hyperparam; \
  typedef \
    distributions_component< distributions::clsname, distributions_ ## clsname ## _message > \
    distributions_ ## clsname ## _component;
#define DISTRIB_INSTANTIATE(clsname, fields, uname) \
  DISTRIB_GENERATE_MESSAGE_CLASS(clsname, fields) \
  DISTRIB_TYPEDEFS(clsname)

#define DISTRIB_BB(x) \
  x(alpha) \
  x(beta)

#define DISTRIB_BNB(x) \
  x(alpha) \
  x(beta) \
  x(r)

#define DISTRIB_GP(x) \
  x(alpha) \
  x(inv_beta)

#define DISTRIB_NICH(x) \
  x(mu) \
  x(kappa) \
  x(sigmasq) \
  x(nu)

// XXX: how do get the macros to do this include automatically?
#include <distributions/models/bb.hpp>
#include <distributions/models/bnb.hpp>
#include <distributions/models/gp.hpp>
#include <distributions/models/nich.hpp>

#define DISTRIB_FOR_EACH_DISTRIBUTION(x) \
  x(BetaBernoulli, DISTRIB_BB, "beta_bernoulli") \
  x(BetaNegativeBinomial, DISTRIB_BNB, "beta_negative_binomial") \
  x(GammaPoisson, DISTRIB_GP, "gamma_poisson") \
  x(NormalInverseChiSq, DISTRIB_NICH, "nich")

DISTRIB_FOR_EACH_DISTRIBUTION(DISTRIB_INSTANTIATE)
