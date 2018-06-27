#_*_ coding:utf-8 _*_
"""
梯度下降法
    
使用梯度下降法前, 最好进行数据归一化:
    在 bobo 老师 6-5 节: 梯度下降法与数据标准化 这一课中, 说明了使用
    梯度下降法要进行数据归一化的必要性, 因为如果不这么做, 可能得到的梯度
    dJ(theta, X_b, y) 会非常大, 此时, 如果步长 eta 比较大的话, 那么 theta
    变化太大, loss function J(theta, X_b, y) 这个函数返回的结果可能是 inf;
    而当 eta 比较小的话, 此时 theta 变化太小, 导致收敛太慢.
    归一化可以解决上述问题.

使用梯度下降法比使用线性回归模型的公式求参数 theta 更快.
在 dJ(theta, X_b, y) 中, 即求 J 对 theta 的梯度, 需要用上 X_b 中全部的样本,
当样本数量较多时, 速度也会比较慢, 解决方法是随机梯度下降法.
dJ 中用到的批量梯度下降法: Batch Gradient Descent
dJ = 2 / m * X_b.transpose() * (X_b * theta - y)
"""

import numpy as np
import matplotlib.pyplot as plt

plot_x = np.linspace(-1, 6, 141)
plot_y = (plot_x - 2.5)**2 - 1


def dJ(theta):
    return 2*(theta - 2.5)

def J(theta):
    return (theta - 2.5)**2 - 1


theta = 0.0
eta = 0.1
epsilon = 1e-8

theta_history = []
theta_history.append(theta)

while True:
    gradient = dJ(theta)
    last_theta = theta
    theta = theta - eta * gradient
    theta_history.append(theta)

    # 要退出循环, 需要找到极值点, 但是没有办法令 dJ(theta) == 0.0
    # 在计算机中使用浮点数相等还是有误差的, 所以应该使用如下方式, 求两个值
    # 的绝对值小于 epsilon
    if (abs(J(theta) - J(last_theta)) < epsilon):
        break

print(theta)
print(J(theta))

# theta 的变化
plt.plot(plot_x, J(plot_x))
plt.plot(np.array(theta_history), J(np.array(theta_history)), 'r-*')


"""
在线性回归模型中使用梯度下降法
"""
np.random.seed(666)
x = np.random.random(size=100) * 2 ## 100 个样本, 每个样本只有一个特征
y = x * 3. + 4. + np.random.normal(size=100)

X = x.reshape(-1, 1)
plt.figure()
plt.scatter(x, y)

## 使用梯度下降法进行训练
def J(theta, X_b, y):
    """
    用于求任意时刻 theta 变化后的 loss, 注意 X_b 相对 X 多了一列, 多出来的
    这一列中全是 1, 具体看多元线性回归梯度下降法的公式
    使用 try 是为了避免 theta 变化太大, 导致 loss 发生很大的变化, 以致无法求值
    """
    try:
        return np.sum((y - np.dot(X_b, theta))**2) / len(X_b)
    except:
        return float('inf')

def dJ(theta, X_b, y):
    # return np.sum((np.dot(X_b, theta) - y).reshape(-1, 1) * X_b, axis=0) * 2. / len(X_b)
    return 2. / len(X_b) * np.dot(X_b.T, (X_b.dot(theta) - y))


def gradient_descent(X_b, y, initial_theta, eta, n_iters=1e4, epsilon=1e-8):
    theta = initial_theta
    i_iter = 0

    while i_iter < n_iters:
        gradient = dJ(theta, X_b, y)
        last_theta = theta
        theta = theta - eta * gradient

        if (abs(J(theta, X_b, y) - J(last_theta, X_b, y)) < epsilon):
            break

        i_iter += 1

    return theta

X_b = np.hstack([np.ones((len(X), 1)), X])
initial_theta = np.zeros(X_b.shape[1])
eta = 0.01

theta = gradient_descent(X_b, y, initial_theta, eta)
print("Batch Gradient Descent: ", theta)


"""
随机梯度下降法
"""
def dJ_sgd(theta, X_b_i, y_i):
    return 2. * np.dot(X_b_i.T, (X_b_i.dot(theta) - y_i))


def sgd(X_b, y, initial_theta, n_iters):
    a = 5
    b = 50

    ## 使用模拟退火的思路获得学习速率
    def learning_rate(i_iter):
        return a / (i_iter + b)

    theta = initial_theta
    ## 在随机梯度下降法中, 循环结束的条件只有迭代次数达到最大的次数,
    ## 这与上面的 gradient(X_b, y,....) 函数的循环终止条件不同, 因为
    ## 由于是 theta 随机变化, 无法保证前后两次的函数值小于 epsilon 就
    ## 说明到达了极值
    for cur_iter in range(n_iters):
        rand_i = np.random.randint(len(X_b)) # 随机选取一个样本
        gradient = dJ_sgd(theta, X_b[rand_i], y[rand_i])
        theta = theta - learning_rate(cur_iter) * gradient

    return theta

## 测试
theta = sgd(X_b, y, initial_theta, n_iters=len(X_b))
print("Stochastic Gradient Descent: ", theta)

"""
scikit-learn 中的 SGD
"""
from sklearn.datasets import load_boston
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.linear_model.stochastic_gradient import SGDRegressor

boston = load_boston()
X = boston.data
y = boston.target

# 去除异常值 50.0 就是 np.max(y), 画散点图可以看出来有异常值
X = X[y < 50.0]
y = y[y < 50.0]

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=111)

standscaler = StandardScaler()
standscaler.fit(X_train)
X_train_standard = standscaler.transform(X_train)
X_test_standard = standscaler.transform(X_test)

sgd_reg  = SGDRegressor(max_iter=100)
sgd_reg.fit(X_train_standard, y_train)
print("R^2 Score: ", sgd_reg.score(X_test_standard, y_test))


"""
如何调试梯度
"""
np.random.seed(666)
X = np.random.random(size=(1000, 10))

true_theta = np.arange(1, 12, dtype=float)

X_b = np.hstack([np.ones((len(X), 1)), X])
y = X_b.dot(true_theta) + np.random.normal(size=len(X))
initial_theta = np.zeros(X_b.shape[1])
eta = 0.01 ## learning rate

## loss 为上面定义的 J(theta, ...)
def dJ_math(theta, X_b, y):
    """
    使用数学推导得到的梯度求法
    """
    return 2. / len(X_b) * np.dot(X_b.T, (X_b.dot(theta) - y))

def dJ_debug(theta, X_b, y, epsilon=0.01):
    """
    最后返回 \partial J/\partial theta, 长度和 theta 相同.
    计算公式为:
    ( J(theta + epsilon) - J(theta - epsilon) ) / (2 * epsilon)
    """
    res = np.empty(len(theta))
    for i in range(len(theta)):
        theta_1 = theta.copy()
        theta_1[i] += epsilon
        theta_2 = theta.copy()
        theta_2[i] -= epsilon

        res[i] = (J(theta_1, X_b, y) - J(theta_2, X_b, y)) / (2 * epsilon)

    return res

def gradient_descent_debug(dJ, X_b, y, initial_theta, eta, n_iters=1e4, epsilon=1e-8):
    theta = initial_theta
    i_iter = 0

    while i_iter < n_iters:
        gradient = dJ(theta, X_b, y)
        last_theta = theta
        theta = theta - eta * gradient

        if (abs(J(theta, X_b, y) - J(last_theta, X_b, y)) < epsilon):
            break

        i_iter += 1

    return theta

theta_debug = gradient_descent_debug(dJ_debug, X_b, y, initial_theta, eta)
print("true_theta: ", true_theta)
print("Use dJ_debug to calculate dJ/d(theta):\n", theta_debug.reshape(-1, 1))
