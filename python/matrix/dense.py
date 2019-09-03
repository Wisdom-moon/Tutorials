import numpy as np

m = np.matrix([[2,3,4], [1,2,3], [1,3,5], [2,4,6]])

print m.shape
print m

m1,m2,m3 = np.hsplit(m, [1,2])

mm = np.hstack((m1,m3))

print 'remove row 2'
print mm.shape
print mm

m1,m2,m3 = np.vsplit(m, [1,2])

mm = np.vstack((m1,m3))

print 'remove column 2'
print mm.shape
print mm
