#_*_ coding:utf-8 _*_
"""
KNN 算法:
    对于每个新的数据点 a, 在现有的所有数据点中, 找到离 a 最近的 k 个数据点,
    然后这 k 个数据点根据自身的 label 对新数据点进行投票, 如果某类的数据点
    最多, 那么新数据点就属于那一类.

KNN 是不需要训练的, 训练数据集就是 KNN 模型本身, 复杂的是预测阶段

判断机器学习算法的性能:
    train_test_split : 将数据集分为训练数据集以及测试数据集
    accuracy_score: 需要 predict_y 以及 y_test

超参数:
    + 领域知识
    + 经验数值
    + 实验探索

数据归一化:
    + 将所有的数据映射到同一尺度
        + 最值归一化(normalization): 把所有数据映射到 0 ~ 1 之间
                    x_scale = (x - x_min) / (x_max - x_min)
            适用于分布有明显边界的情况; 受 outlier 影响较大
            比如说用于处理学生成绩(0 ~ 100), 图像像素(0 ~ 255) 这些是有边界的情况.
            然而对于没有明显边界的情况, 比如人们的收入, 有的人收入极大, 如果按上述
            方式归一化的话, 结果容易受极值影响, 产生偏差.

        + 均值方差归一化(standardization): 把所有数据归一到均值为 0 方差为 1 的分布中
                    x_scale = (x - x_mean) / S
            数据分布没有明显的边界; 有可能存在极端数据值
            一般情况都使用这种归一化

测试数据归一化:
    (x_test - mean_train) / std_train
    也就是说, 使用训练数据集中的均值和方差进行 standardization
    原因: 测试数据是模拟真实环境, 真实环境很有可能无法得到所有测试数据的均值和方差

k近邻算法的思考:
    + k近邻算法可以处理多分类问题
    + k近邻算法还可以用来解决回归问题:
        新来的样本点的值是最近的 k 个样本的值的平均值: kNeighborsRegressor
    + 缺点:
        + KNN 算法效率低下
        + 高度数据相关
        + 预测结果不具有可解释性
        + 维数灾难: 随着维度的增加, "看似相近" 的两个点之间的距离越来越大
                解决方法: 降维
"""
import numpy as np
from collections import Counter
X_train = np.array([[3.393533211, 2.331273381],
              [3.110073483, 1.781539638],
              [1.343808831, 3.368360954],
              [3.582294042, 4.679179110],
              [2.280362439, 2.866990263],
              [7.423436942, 4.696522875],
              [5.745051997, 3.533989803],
              [9.172168622, 2.511101045],
              [7.792783481, 3.424088941],
              [7.939820817, 0.791637231]
             ])
y_train = np.array([0, 0, 0, 0, 0, 1, 1, 1, 1, 1])

# 新来的样本
x = np.array([8.093607318, 3.365731514])

# 计算new sample 与现有样本的距离, 找到最近的 k 个样本
distances = [np.sum((x - x_train) ** 2) for x_train in X_train]
nearest = np.argsort(distances)
k = 6

topK_y = y_train[nearest[:6]]
print("topK_y: ", topK_y)
votes = Counter(topK_y) # 对数组中的元素进行统计
print("votes: ", votes)
# 找到票数最多的 1 个元素, most_common 返回的结果为 [(1, 5)]
predict_y = votes.most_common(1)[0][0]
print("predict: ", predict_y)


"""
scikit-learn 中的 train_test_split
    test_size=0.2 # test 集的比例
"""
from sklearn.datasets import load_iris, load_digits
from sklearn.model_selection import train_test_split
iris = load_iris()
X = iris.data
y = iris.target
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
print(X_train.shape, y_train.shape) # (120, 4), (120,)
print(X_test.shape, y_test.shape) # (30, 4), (30,)


"""
scikit-learn 中的 KNN
+ KNeighborsClassifier(n_neighbors=k)
    .fit(X_train, y_train)
    .predict(X_test)
    .score(X_test, y_test)
+ accuracy_score(y_test, predict_y)
"""
digits = load_digits()
X = digits.data
y = digits.target
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score
kNN_classifier = KNeighborsClassifier(n_neighbors=6)
print(kNN_classifier)
# 拟合的过程
kNN_classifier.fit(X_train, y_train)
accuracy = kNN_classifier.score(X_test, y_test)
print("accuracy1: ", accuracy)
predict_y = kNN_classifier.predict(X_test)
accuracy = accuracy_score(y_test, predict_y)
print("accuracy2: ", accuracy)


"""
寻找最好的 k
还有超参数: weights : `uniform` or `distance`
            距离的选择,
            相应的 p, p=2 为欧拉距离

寻找超参数的方法: 网格搜索
"""
best_score = 0.0
best_k = -1
for k in range(2, 11):
    knn_clf = KNeighborsClassifier(n_neighbors=k,
                                   weights='distance',
                                   metric='minkowski',
                                   p=2)
    knn_clf.fit(X_train, y_train)
    score = knn_clf.score(X_test, y_test)
    if score > best_score:
        best_score = score
        best_k = k

print("========================= find best k:")
print("best_score: ", best_score)
print("best_k: ", best_k)


"""
数据归一化处理
自己进行处理
"""
x = np.random.randint(0, 100, size=100)
x_nor = (x - np.min(x)) / (np.max(x) - np.min(x))

X = np.random.randint(0, 100, size=(10, 5))
X = np.array(X, dtype=float)
X_nor = (X - np.min(X, axis=0)) / (np.max(X, axis=0) - np.min(X, axis=0))
print(X_nor.shape)

X2 = np.random.randint(0, 100, size=(50, 2))
X2 = np.array(X2, dtype=float)
X2_std = (X2 - np.mean(X2, axis=0)) / np.std(X2, axis=0)
# import matplotlib.pyplot as plt
# plt.figure()
# plt.scatter(X2_std[:, 1], X2_std[:, 0])
# plt.show()

"""
scikit-learn 中的 Scalar, 用于数据归一化
StandardScalar:
    .fit(X_train)  # 求取 X_train 中的 mean_ 和 scale_
    .transfrom(X_test) # 对 X_test 使用 mean_ 和 scale_ 归一化
"""
from sklearn.preprocessing import StandardScaler
X = iris.data
y = iris.target

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
standardScaler = StandardScaler()
standardScaler.fit(X_train)
print(standardScaler.mean_)
# std_ 被废弃了, scale_ 描述数据的分布范围, std_ 只是描述数据分布范围的比较常用
# 的指标之一
print(standardScaler.scale_)
X_train = standardScaler.transform(X_train)
X_test = standardScaler.transform(X_test)

kNN_classifier = KNeighborsClassifier(n_neighbors=6)
kNN_classifier.fit(X_train, y_train)
accuracy = kNN_classifier.score(X_test, y_test)
print("accuracy1: ", accuracy)

predict_y = kNN_classifier.predict(X_test)
accuracy = accuracy_score(y_test, predict_y)
print("accuracy2: ", accuracy)
