from microscopes.hodgepodge import *

from sklearn.datasets import fetch_mldata
mnist_dataset = fetch_mldata('MNIST original')

import numpy as np
import numpy.ma as ma
import matplotlib.pylab as plt
import math

if __name__ == '__main__':
    Y = mnist_dataset['data'][np.where(mnist_dataset['target'] == 2.)[0]]
    #Y = mnist_dataset['data']
    N, D = Y.shape
    W = int(math.sqrt(D))
    assert W * W == D
    dtype = [('', bool)]*D
    inds = np.random.permutation(np.arange(N))[:300]
    Y = np.array([tuple(y) for y in Y[inds]], dtype=dtype)

    #inds = np.random.permutation(np.arange(N))[:2]
    #Y[1] = tuple(not e for e in Y[0])

    #D=3
    #dtype = [('', bool)]*D
    #Y = np.array([(False, True, False), (True, True, False), (True, False, True)], dtype=dtype)

    view = numpy_dataview(Y)
    mm = mixturemodel(Y.shape[0], {'alpha':50.0}, ['beta_bernoulli']*D, [{'alpha':1., 'beta':1.}]*D)
    rng = rng_t(543)
    bootstrap(mm, view, rng)
    print 'training with dataset size', Y.shape[0], 'nfeatures', D
    for i in xrange(2000):
        gibbs_assign(mm, view, rng)
