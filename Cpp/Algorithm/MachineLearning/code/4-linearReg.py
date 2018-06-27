#_*_ coding:utf-8 _*_
"""
线性回归算法:
    寻找一条直线, 最大程度的 "拟合" 样本特征和样本输出标记之间的关系

分类问题的横纵坐标都是样本特征, 而回归问题横轴是样本特征,
纵轴是样本的 label(比如房屋的面积和价格).

线性回归算法的评测:
    如果使用均方误差 MSE 的话, 量纲上可能会有问题, 所以, 一般使用
    1. 根均方误差: RMSE (Root Mean Squared Error)
        sqrt(MSE)

    2. 平均绝对误差: MAE(Mean Absolute Error)
        1/m sum_{1}^{m} |yi - yi_predict|

    3. R Squared:
"""

"""
实现 simple linear regression
"""
import numpy as np
import matplotlib.pyplot as plt


x = np.array([1., 2., 3., 4., 5.])
y = np.array([1., 3., 2., 3., 5.])

fig1 = plt.figure()
plt.scatter(x, y)
plt.axis([0, 6, 0, 6])

x_mean = np.mean(x)
y_mean = np.mean(y)

numerator = np.dot((x - x_mean), (y - y_mean))
denominator = np.dot((x - x_mean), (x - x_mean))

a = numerator / denominator
b = y_mean - a * x_mean
print(a.dtype, b)

y_hat = a * x + b
plt.plot(x, y_hat, color='r')

"""
衡量回归算法的标准
"""

from sklearn.datasets import load_boston
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.metrics import mean_absolute_error
from sklearn.metrics import r2_score

boston = load_boston()

x = boston.data[:, 5] # 只使用房间数量的特征
y = boston.target

x = x[y < np.max(y)]
y = y[y < np.max(y)]

fig2 = plt.figure()
plt.scatter(x, y)

x_train, x_test, y_train, y_test = train_test_split(x, y, random_state=666)
x_train = x_train.reshape(-1, 1)
y_train = y_train.reshape(-1, 1)
x_test = x_test.reshape(-1, 1)
y_test = y_test.reshape(-1, 1)
reg = LinearRegression()
reg.fit(x_train, y_train)
score1 = reg.score(x_test, y_test)
y_predict = reg.predict(x_test)
score2 = r2_score(y_test, y_predict)
print("R^2 Score 1: ", score1)
print("R^2 Score 2: ", score2)

"""
多元线性回归模型
"""
X = boston.data # 使用所有的特征
Y = boston.target

X_train, X_test, y_train, y_test = train_test_split(X, Y, random_state=666)

lin_reg = LinearRegression()
lin_reg.fit(X_train, y_train)

score1 = lin_reg.score(X_test, y_test)
y_predict = lin_reg.predict(X_test)
score2 = r2_score(y_test, y_predict)
print("R^2 Score 1: ", score1)
print("R^2 Score 2: ", score2)

"""
kNN Regressor
"""
from sklearn.neighbors import KNeighborsRegressor

knn_reg = KNeighborsRegressor(n_neighbors=5)
knn_reg.fit(X_train, y_train)

print("kNN Score: ", knn_reg.score(X_test, y_test))

"""
对 KNN 使用网格搜索
"""
from sklearn.model_selection import GridSearchCV # CV 表示 Cross Validation

param_grid = [
    {
        "weights": ["uniform"],
        "n_neighbors": [i for i in range(1, 11)]
    },
    {
        "weights": ["distance"],
        "n_neighbors": [i for i in range(1, 11)],
        "p": [i for i in range(1, 6)]
    }
]

knn_reg = KNeighborsRegressor()
grid_search = GridSearchCV(knn_reg, param_grid, n_jobs=-1, verbose=1)
grid_search.fit(X_train, y_train)

print(grid_search.best_params_)
print("cv_best_score: ", grid_search.best_score_)
print("r2_score: ", grid_search.best_estimator_.score(X_test, y_test))
