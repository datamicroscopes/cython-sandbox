from microscopes.hodgepodge import *
import numpy as np
import numpy.ma as ma

if __name__ == '__main__':
    #d = 2
    #x = np.array([(False, True), (True, False)], dtype=[('', bool)]*d)
    #view = numpy_dataview(x)
    #rng = rng_t(1243)
    #mm = mixturemodel(x.shape[0], {'alpha':2.0}, ['beta_bernoulli']*d, [{'alpha':1., 'beta':1.}]*d)
    #bootstrap(mm, view, rng)
    #gibbs_assign(mm, view, rng)

    x = np.array([(False, 0.1), (True, 10.0)], dtype=[('',bool), ('',np.float)])
    view = numpy_dataview(x)
    rng = rng_t(1243)
    mm = mixturemodel(x.shape[0], {'alpha':2.0}, ['beta_bernoulli', 'nich'], [{'alpha':1., 'beta':1.}, {'mu': 0., 'kappa': 1., 'sigmasq': 1., 'nu': 1.}])
    bootstrap(mm, view, rng)
    gibbs_assign(mm, view.permute(rng), rng)

    y_new = ma.masked_array( np.array([(True, 30.0)], dtype=[('', np.bool), ('', np.float)]), mask=[(False, True)] )
    print mm.sample_post_pred(y_new, rng, size=10)

    x1 = ma.masked_array( np.array([(True, False, True, True, True)], dtype=[('', np.bool)]*5), mask=[(False, False, True, True, True)] )
    view1 = numpy_dataview(x1)
