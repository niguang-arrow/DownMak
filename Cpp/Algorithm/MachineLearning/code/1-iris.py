from sklearn import datasets
import numpy as np
import matplotlib.pyplot as plt
from pprint import pprint as pp

iris = datasets.load_iris()
# dict_keys(['DESCR', 'target', 'data', 'feature_names', 'target_names'])
print(iris.keys())
# print(iris.DESCR)
print(iris.data.shape)
pp(iris.feature_names)
print(iris.target.shape)
print(iris.target_names)


X = iris.data
y = iris.target
feature_num = X.shape[1]
choices = [(i, j) for i in range(feature_num) for j in range(feature_num) if j > i]
pp(choices)
markers = "o+x"
colors = 'rgb'

fig = plt.figure()
for target, c, m in zip(np.unique(iris.target), colors, markers):
    for idx, (i, j) in enumerate(choices, 1):
        ax = fig.add_subplot(2, 3, idx)
        ax.scatter(X[y==target, i], X[y==target, j], color=c, marker=m)
plt.show()
