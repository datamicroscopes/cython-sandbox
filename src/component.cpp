#include "component.hpp"
#include <stdexcept>

using namespace std;

function<shared_ptr<component>(const hyperparam_t &)>
component::metafactory(const string &name)
{
  if (name == "beta_bernoulli")
    return [](const hyperparam_t &) {
      return std::make_shared<beta_bernoulli>();
    };
  throw runtime_error("unknown component name");
}
