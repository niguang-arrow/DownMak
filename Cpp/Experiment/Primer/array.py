import time
import numpy as np


t1 = time.time()
a = np.random.rand(1000000)
t2 = time.time()
print t2 - t1
