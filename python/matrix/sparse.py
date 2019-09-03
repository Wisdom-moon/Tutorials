import numpy as np
from scipy.sparse import csr_matrix

m = csr_matrix((4,5), dtype=np.int8)

print m.shape

m_a = m.toarray()
print m_a

m_d = m.todense()
print m_d
