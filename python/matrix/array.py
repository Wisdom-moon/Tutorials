import numpy as np

#create a array setting values to zero
tuple_array_0 = np.zeros(100, dtype=[('x', float),('y', float)])
#create a array with shape (30,20), filled with  np.inf, type is tuple
tuple_array = np.full((30,20), np.inf, dtype=[('x', int),('y', int)])

#Fill the array with a scalar value.
tuple_array_0.fill(0.0)

#Load data from a text file.
m_data = np.loadtxt('matrix')
