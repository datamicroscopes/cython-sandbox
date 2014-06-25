#include "component.hpp"
#include <stdexcept>
#include <iostream>
#include <cmath>

using namespace std;

pair<component::make_hyperparam_fn_t, component::make_component_fn_t >
component::metafactory(const string &name)
{
#define DISTRIB_FACTORY(clsname, fields, uname) \
  if (name == uname) { \
    make_hyperparam_fn_t fn0 = [](const hyperparam_bag_t &bag) { \
      const auto msg = distributions_ ## clsname ## _message::from_bag(bag); \
      return make_shared< distributions_ ## clsname ## _hyperparam >(msg); \
    }; \
    make_component_fn_t fn1 = [](const hyperparam &hp, rng_t &rng) { \
      const auto &hp0 =  \
          static_cast< const distributions_ ## clsname ## _hyperparam & >(hp); \
      return make_shared< distributions_ ## clsname ## _component >(hp0, rng); \
    }; \
    return make_pair(fn0, fn1); \
  }

DISTRIB_FOR_EACH_DISTRIBUTION(DISTRIB_FACTORY)

  throw runtime_error("unknown component name: " + name);
}
