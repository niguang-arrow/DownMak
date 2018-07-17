#_*_ coding:utf-8 _*_
from sklearn import datasets
import numpy as np
import matplotlib.pyplot as plt
from pprint import pprint as pp

iris = datasets.load_iris()

X = iris.data[:, 2:]
y = iris.target

from sklearn.tree import DecisionTreeClassifier

dt_clf = DecisionTreeClassifier(max_depth=2, criterion='entropy', random_state=42)
dt_clf.fit(X, y)

"""
决策边界
"""
def plot_decision_boundary(model, axis):
    
    x0, x1 = np.meshgrid(
        np.linspace(axis[0], axis[1], int((axis[1]-axis[0])*100)).reshape(-1, 1),
        np.linspace(axis[2], axis[3], int((axis[3]-axis[2])*100)).reshape(-1, 1),
    )
    X_new = np.c_[x0.ravel(), x1.ravel()]

    y_predict = model.predict(X_new)
    pp(y_predict)
    zz = y_predict.reshape(x0.shape)

    from matplotlib.colors import ListedColormap
    custom_cmap = ListedColormap(['#EF9A9A','#FFF59D','#90CAF9'])
    
    plt.contourf(x0, x1, zz, cmap=custom_cmap)

plot_decision_boundary(dt_clf, axis=[0.5, 7.5, 0, 3])

"""
信息熵
"""

def entropy(p):
    return -p * np.log(p) - (1 - p) * np.log(1 - p)

x = np.linspace(0.01, 0.99, 200)
plt.figure()
plt.plot(x, entropy(x))
# plt.show()

"""
模拟使用信息熵划分
"""

"""
基于维度 d 进行划分
"""
def split(X, y, d, value):
    index_a = (X[:, d] <= value)
    index_b = (X[:, d] > value)
    return X[index_a], X[index_b], y[index_a], y[index_b]


from collections import Counter
def entropy(y):
    counter = Counter(y)
    res = 0.0
    for num in counter.values():
        p = num / len(y)
        res += -p * np.log(p)
    return res

def try_split(X, y):

    best_entropy = float('inf')
    best_d, best_v = -1, -1

    for d in range(X.shape[1]):
        sorted_index = np.argsort(X[:, d])
        for i in range(1, len(X)):
            if (X[sorted_index[i - 1], d] != X[sorted_index[i], d]):
                v = (X[sorted_index[i - 1], d] + X[sorted_index[i], d]) / 2
                X_l, X_r, y_l, y_r = split(X, y, d, v)
                e = entropy(y_l) + entropy(y_r)
                if e < best_entropy:
                    best_entropy, best_d, best_v = e, d, v

    return best_entropy, best_d, best_v

best_entropy, best_d, best_v = try_split(X, y)
print("best_entropy: ", best_entropy)
print("best_d: ", best_d)
print("best_v: ", best_v)

X1_l, X1_r, y1_l, y1_r = split(X, y, best_d, best_v)
print("entropy: ", entropy(y1_l))
print("entropy: ", entropy(y1_r))

best_entropy, best_d, best_v = try_split(X1_r, y1_r)
print("best_entropy: ", best_entropy)
print("best_d: ", best_d)
print("best_v: ", best_v)

X2_l, X2_r, y2_l, y2_r = split(X, y, best_d, best_v)
print("entropy: ", entropy(y2_l))
print("entropy: ", entropy(y2_r))
