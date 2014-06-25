#include "kernel.hpp"

using namespace std;

default_random_engine gibbs::Prng_;
uniform_real_distribution<float>
gibbs::Unif01_(0.0, 1.0);
