from microscopes.hodgepodge import *
import numpy as np

if __name__ == '__main__':
    d = 2
    x = np.array([(False, True), (True, False)], dtype=[('', bool)]*d)
    view = numpy_dataview(x)
    rng = rng_t(1243)
    mm = mixturemodel(x.shape[0], {'alpha':2.0}, ['beta_bernoulli']*d, [{'alpha':1., 'beta':1.}]*d)
    bootstrap(mm, view, rng)
    gibbs_assign(mm, view, rng)
