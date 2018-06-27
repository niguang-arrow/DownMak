#_*_ coding:utf-8 _*_
"""
分类准确度的问题:
    对于极度偏斜(Skewed Data) 的数据, 只使用分类准确度是远远不够的.
    使用混淆矩阵(Confusion Matrix)做进一步的分析

Confusion Matrix:
    + 行表示真实值, 列表示预测值
                 0        1
            -------------------
            0   TN       FP
            1   FN       TP
            -------------------
        T 表示 True, N 表示 Negative
        F 表示 False, P 表示 Positive
        TN: 预测 Negative 正确
        FP: 预测 Positive 错误
        FN: 预测 Negative 错误
        TP: 预测 Positive 正确

精准率和召回率:
    通过混淆矩阵得到.
    在极其有偏的数据中, 我们不看准确率, 而是看精准率和召回率, 它们才能更好的评价
    分类系统的好坏.
    + 精准率:
        precision = TP / (TP + FP)
        将分类 1 作为关注的对象, 精准率就是求出我们关注内容的预测准确率.
        上面公式就是表格的第二列. (相当于样本中真实值有 0 也有 1, 注意与下面
        的召回率区分, 在召回率中, 样本中的真实值只含有 1.)
    + 召回率:
        recall = TP / (TP + FN)
        我们关注的事件已经发生了, 召回率就是在这些已经发生的事件中, 我们关注
        内容预测的准确率.
        上面公式的第二行

如何看待精准率和召回率
    + 有的时候我们注重精准率, 如股票预测. 
    + 有的时候我们注重召回率, 如病人预测: 希望将所有有病的人给预测出来,
    精准率低一些没有关系.

F1 Score:
    F1 Score 是 precision 和 recall 的调和平均值
    也就是 precision 的倒数和 recall 的倒数之和再取平均. 
        1 / F1 = 1/2 (1/precision + 1/recall)
    如果 precision 和 recall 的值不均衡(比如说一个特别大, 另一个特别小), 那么
    F1 Score 就会很小, 只有这它们俩都很高的话才能得到较高的值.
        F1 = (2 * precision * recall) / (precision + recall)

Precision-Recall 的平衡:
    它们是相互矛盾的两个指标.
"""
import numpy as np
from sklearn.datasets import load_digits
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression

digits = load_digits()
X = digits.data
y = digits.target.copy() # 没有 .copy() 的话, 改变 y 会同时影响 digits.target

"""
产生偏斜数据, 大量的数据的 label 是 0
"""
y[digits.target == 9] = 1
y[digits.target != 9] = 0

X_train, X_test, y_train, y_test = train_test_split(X, y, random_state=666)

log_reg = LogisticRegression()
log_reg.fit(X_train, y_train)
accuracy = log_reg.score(X_test, y_test)
print("accuracy: ", accuracy)

y_log_predict = log_reg.predict(X_test)


"""
scikit-learn 中的混淆矩阵, 精准率和召回率
"""
from sklearn.metrics import (precision_score,
                             recall_score,
                             confusion_matrix)
print("混淆矩阵: \n", confusion_matrix(y_test, y_log_predict))
print("精准率: ", precision_score(y_test, y_log_predict))
print("召回率: ", recall_score(y_test, y_log_predict))


"""
f1 score
"""
from sklearn.metrics import f1_score
print("F1 Score: ", f1_score(y_test, y_log_predict))
print()

"""
精准率和召回率的平衡
"""
decision_score = log_reg.decision_function(X_test)
print("min: ", np.min(decision_score))
print("max: ", np.max(decision_score))
threshold = 5
y_predict_2 = np.array(decision_score >= 5, dtype=int)
print("混淆矩阵: \n", confusion_matrix(y_test, y_predict_2))
print("精准率: ", precision_score(y_test, y_predict_2))
print("召回率: ", recall_score(y_test, y_predict_2))

def TN(y_true, y_predict):
    assert len(y_true) == len(y_predict)
    return np.sum((y_true == 0) & (y_predict == 0))

def FP(y_true, y_predict):
    assert len(y_true) == len(y_predict)
    return np.sum((y_true == 0) & (y_predict == 1))

def FN(y_true, y_predict):
    assert len(y_true) == len(y_predict)
    return np.sum((y_true == 1) & (y_predict == 0))

def TP(y_true, y_predict):
    assert len(y_true) == len(y_predict)
    return np.sum((y_true == 1) & (y_predict == 1))

def confusion_matrix(y_true, y_predict):
    return np.array([
        [TN(y_true, y_predict), FP(y_true, y_predict)],
        [FN(y_true, y_predict), TP(y_true, y_predict)]
    ])


def precision_score(y_true, y_predict):
    tp = TP(y_true, y_predict)
    fp = FP(y_true, y_predict)
    try:
        return tp / (tp + fp)
    except:
        return 0.0

def precision_score(y_true, y_predict):
    tp = TP(y_true, y_predict)
    fn = FN(y_true, y_predict)
    try:
        return tp / (tp + fn)
    except:
        return 0.0
