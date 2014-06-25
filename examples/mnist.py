from microscopes.hodgepodge import *

from sklearn.datasets import fetch_mldata
mnist_dataset = fetch_mldata('MNIST original')

import numpy as np
import numpy.ma as ma
import matplotlib.pylab as plt
import math

if __name__ == '__main__':
    Y = mnist_dataset['data'][np.where(mnist_dataset['target'] == 2.)[0]]
    N, D = Y.shape
    W = int(math.sqrt(D))
    assert W * W == D
    dtype = [('', bool)]*D
    inds = np.random.permutation(np.arange(N))
    Y = np.array([tuple(y) for y in Y[inds]], dtype=dtype)

    #D=3
    #dtype = [('', bool)]*D
    #Y = np.array([(False, True, False), (True, True, False)], dtype=dtype)

    view = numpy_dataview(Y)
    mm = mixturemodel(Y.shape[0], {'alpha':10.0}, ['beta_bernoulli']*D, [{'alpha':1., 'beta':1.}]*D)
    bootstrap(mm, view)
    print 'training with dataset size', Y.shape[0]
    for i in xrange(500):
        gibbs_assign(mm, view)
