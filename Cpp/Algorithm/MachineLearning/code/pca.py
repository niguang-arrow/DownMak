import numpy as np

data = np.array([
    [2.5, 0.5, 2.2, 1.9, 3.1, 2.3, 2, 1, 1.5, 1.1],
    [2.4, 0.7, 2.9, 2.2, 3.0, 2.7, 1.6, 1.1, 1.6, 0.9]
]).T

data = data - data.mean(axis=0)
print(data)
print(np.sum(data ** 2, axis=0) / (len(data) - 1))
print(np.sum(data[:, 0] * data[:, 1], axis=0) / (len(data) - 1))
print(np.cov(data, rowvar=False))
