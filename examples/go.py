from microscopes.cxx.common.dataview import numpy_dataview
from microscopes.cxx.common.rng import rng
from microscopes.cxx.models import bb
from microscopes.cxx.mixture.model import state
from microscopes.cxx.kernels.bootstrap import likelihood
from microscopes.cxx.kernels.gibbs import assign

import numpy as np
import numpy.ma as ma

if __name__ == '__main__':
    r = rng(12345)
    n = 2
    s = state(n, [bb, bb])

    s.set_hp({'alpha':2.0})
    s.set_feature_hp(0, {'alpha':1.0,'beta':1.0})
    s.set_feature_hp(1, {'alpha':1.1,'beta':1.1})

    print s.get_hp()
    print s.get_feature_hp(0)
    print s.get_feature_hp(1)

    x = np.array([(False, True), (True, False)], dtype=[('', bool)]*2)
    view = numpy_dataview(x)
    likelihood(s, view, r)

    for _ in xrange(10):
        assign(s, view, r)

    #d = 2

    #view = numpy_dataview(x)
    #rng = rng_t(1243)
    #mm = mixturemodel(x.shape[0], {'alpha':2.0}, ['beta_bernoulli']*d, [{'alpha':1., 'beta':1.}]*d)
    #bootstrap(mm, view, rng)
    #gibbs_assign(mm, view, rng)

    #x = np.array([(False, 0.1), (True, 10.0)], dtype=[('',bool), ('',np.float)])
    #view = numpy_dataview(x)
    #rng = rng_t(1243)
    #mm = mixturemodel(x.shape[0], {'alpha':2.0}, ['beta_bernoulli', 'nich'], [{'alpha':1., 'beta':1.}, {'mu': 0., 'kappa': 1., 'sigmasq': 1., 'nu': 1.}])
    #bootstrap(mm, view, rng)
    #gibbs_assign(mm, view.permute(rng), rng)

    #y_new = ma.masked_array( np.array([(True, 30.0)], dtype=[('', np.bool), ('', np.float)]), mask=[(False, True)] )
    #print mm.sample_post_pred(y_new, rng, size=10)

    #x1 = ma.masked_array( np.array([(True, False, True, True, True)], dtype=[('', np.bool)]*5), mask=[(False, False, True, True, True)] )
    #view1 = numpy_dataview(x1)
