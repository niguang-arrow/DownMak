#_*_ coding:utf-8 _*_
"""
PCA: 主成分分析, 最大化样本之间的方差, 使用梯度上升法求解

PCA 不能使用 StandardScaler 对数据标准化, 否则方差就为 1
"""

import numpy as np
import matplotlib.pyplot as plt

X = np.empty((100, 2))
X[:, 0] = np.random.uniform(0., 100., size=len(X))
X[:, 1] = 0.75 * X[:, 0] + 3. + np.random.normal(0., 10., size=len(X))

# plt.figure()
# plt.scatter(X[:, 0], X[:, 1])
# plt.show()

"""
demean
"""
def demean(X):
    return X - np.mean(X, axis=0)

X_demean = demean(X)
# plt.figure()
# plt.scatter(X_demean[:, 0], X_demean[:, 1])
# plt.show()


"""
梯度上升法
"""
def f(w, X):
    return np.sum((X.dot(w))**2) / len(X)

def df_math(w, X):
    return 2. / len(X) * X.T.dot(X.dot(w))

## 判断求导有没有求错
## 由于 w 是一个方向向量, 每一个维度都比较小, 所以 epsilon 的值也比较小
def df_debug(w, X, epsilon=0.0001):
    res = np.empty(len(w))
    for i in range(len(w)):
        w_1 = w.copy()
        w_1[i] += epsilon
        w_2 = w.copy()
        w_2[i] -= epsilon
        res[i] = (f(w_1, X) - f(w_2, X)) / (2 * epsilon)
    return res


def direction(w):
    return w / np.linalg.norm(w)

def gradient_ascent(df, X, initital_w, eta, n_iters=1e4, epsilon=1e-8):
    w = direction(initital_w)
    cur_iter = 0

    while cur_iter < n_iters:
        gradient = df(w, X)
        last_w = w
        w = w + eta * gradient
        w = direction(w) # 每次求一个单位向量

        # 爬坡爬不动了
        if ((abs(f(w, X) - f(last_w, X)) < epsilon)):
            break

        cur_iter += 1

    return w


# 初始的 w 是不能为 0 向量的, 比较要成为单位向量
initital_w = np.random.random(X.shape[1])
eta = 0.001

w1 = gradient_ascent(df_debug, X_demean, initital_w, eta)
w2 = gradient_ascent(df_math, X_demean, initital_w, eta)

print("debug: ", w1)
print("math: ", w2)

X2 = np.empty(shape=(100, 2))
X2[:, 0] = np.random.uniform(0., 100., size=len(X2))
X2[:, 1] = 0.8 * X2[:, 0] + 30.

X2_demean = demean(X2)
w3 = gradient_ascent(df_math, X2_demean, initital_w, eta)
print("w3", w3)
# plt.figure()
# plt.scatter(X2_demean[:, 0], X2_demean[:, 1])
# plt.plot([0, 30 * w3[0]], [0, 30 * w3[1]], color='r')

"""
第一主成分
"""
def first_component(X, initital_w, eta, n_iters=1e4, epsilon=1e-8):
    w = direction(initital_w)
    cur_iter = 0

    while cur_iter < n_iters:
        gradient = df_math(w, X)
        last_w = w
        w = w + eta * gradient
        w = direction(w) # 每次求一个单位向量

        # 爬坡爬不动了
        if ((abs(f(w, X) - f(last_w, X)) < epsilon)):
            break
        cur_iter += 1

    return w

X = demean(X)
initital_w = np.random.random(X.shape[1])
w = first_component(X, initital_w, eta)

## 把第一个维度的主成分去掉之后
X2 = X - X.dot(w).reshape(-1, 1) * w
# plt.figure()
# plt.scatter(X2[:, 0], X2[:, 1])
# plt.show()

w2 = first_component(X2, initital_w, eta)
print("w2: ", w2)
print("w |- w2 ? res: ", w.dot(w2))


"""
scikit-learn 中的 PCA
"""
from sklearn.decomposition import PCA
from sklearn.datasets import load_digits
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier

digits = load_digits()
X = digits.data
y = digits.target

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=666)


knn_clf = KNeighborsClassifier()
knn_clf.fit(X_train, y_train)
print("before pca, knn score: ", knn_clf.score(X_test, y_test))

pca = PCA(n_components=2)
pca.fit(X_train)
X_train_reduction = pca.transform(X_train)
X_test_reduction = pca.transform(X_test)

knn_clf.fit(X_train_reduction, y_train)
print("after pca, knn score: ", knn_clf.score(X_test_reduction, y_test))

# 对于每一个主成分, 它可以解释的原数据的方差比例是多少
ratio_ = pca.explained_variance_ratio_
print(ratio_)

pca = PCA(n_components=X_train.shape[1])
pca.fit(X_train)
plt.plot([i for i in range(X_train.shape[1])],
         [np.sum(pca.explained_variance_ratio_[:i+1]) for i in range(X_train.shape[1])])
# plt.show()
