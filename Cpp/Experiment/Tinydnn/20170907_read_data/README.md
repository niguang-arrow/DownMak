# 直接读取 MNIST 数据

## 2017 年 9 月 7 日

+ 首先, 在 mnist 的官网上仔细研究文件中数据的保存方式; 官网上的数据是在大端法机器上
保存的, 但是我的电脑是按小端法保存数据的, 因此, 需要用 `is_little_endian()` 函数判断
是不是小端法, 如果是的话, 需要使用 `reverse_endian()` 方法转换为大端法. 通过对比 magic
number, 判断数据是否正确.

+ 使用 `ifs.read()` 读取数据时, 需要将第一个参数的类型转换为 `char*`;

+ 原 tiny-dnn 在保存所有图像数据时使用 `std::vector<vec_t>`, 它的 `vec_t` 也是一个
vector, 不过定制了自己的 allocator.
