#_*_coding:utf-8_*_
import random

def random_int_list(start,stop,length):
    '''生成随机整数数组'''
    start, stop = (int(start),int(stop)) if start <= stop else(int(stop),int(start))
    length = int(abs(length)) if length else 0
    random_List = []
    for i in range(length):
        random_List.append(random.randint(start,stop))
    return random_List
arr = []
def bubble(arr):
    '''冒泡排序：'''
    length = len(arr)
    for i in range(0,length-1):
        for j in range(length-1, i, -1):
            if arr[j] <arr[j-1]:
                arr[j], arr[j - 1] = arr[j - 1], arr[j]
                # temp = arr[j]
                # arr[j] = arr[j-1]
                # arr[j-1] = temp
            # else:
                # continue
    return arr

def SelectinSort(arr):
    '''选择排序'''
    length = len(arr)
    for i in range(0,length):
        min = i
        for j in range(i+1,length):
            if arr[j] < arr[j-1]:
                min = j
        if min != i:
            temp = arr[i]
            arr[i] = arr[min]
            arr[min] = temp
    return arr

def InsertionSort(arr):
    '''插入排序'''
    length = len(arr)
    for i in range(1,length):
        for j in range(i,0,-1):
            if arr[j] < arr[j - 1]:
                temp = arr[j]
                arr[j] = arr[j - 1]
                arr[j-1] = temp
            else:
                break
    return arr

def MergeSort(arr):
    '''归并排序'''
    length = len(arr)
    if length <= 1:
        return arr
    middle = length // 2
    left = MergeSort(arr[:middle])
    right = MergeSort(arr[middle:])
#     return Merge(left, right)
# def Merge(left,right):
    l,r = 0,0
    result = []
    while l < len(left) and r < len(right):
        if left[l] < right[r]:
            result.append(left[l])
            l += 1
        else:
            result.append(right[r])
            r += 1
    if l < len(left):
        result = result + left[l:]
    if r < len(right):
        result = result + right[r:]
    return result

if __name__ == "__main__":
    arr = random_int_list(0,100,20)
    print arr
    print '冒泡排序：',bubble(arr)
    print '选择排序：',SelectinSort(arr)
    print '插入排序：',InsertionSort(arr)
    print '归并排序：',MergeSort(arr)
