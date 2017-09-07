#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> // std::reverse
#include <iomanip> // std::setprecision 控制浮点数的精度

typedef double double_t;
typedef std::size_t label_t;
static const std::string data_dir_path = "./data";

struct mnist_header {
    uint32_t magic_number;
    uint32_t num_items;
    uint32_t num_rows;
    uint32_t num_cols;
};

// 官网下载 mnist 文件是按大端法(high endian)存储的, 我这台机器是按小端法
// 存储数据, 因此需要下面的函数进行转换.
template <typename T>
T* reverse_endian(T* p) {
    std::reverse(reinterpret_cast<char*>(p), reinterpret_cast<char*>(p) + sizeof(T));
    return p;
}
inline bool is_little_endian() {
    // int 是 4 个字节, 如果是小端法的话, 表示为 0x01000000
    // 使用 *(char*)&x 得到的结果是 1, 由于不等于 0, 所以为小端法
    int x = 1;
    return *(char*) &x != 0;
}

/* 处理 mnist 数据集的 header, 详情还可以参考 mnist 的官网, 上面有介绍
 * 文件中各个字节到底有什么意义
 */
inline void parse_mnist_header(std::ifstream &ifs, mnist_header &header) {
    ifs.read((char*) &header.magic_number, 4);
    ifs.read((char*) &header.num_items, 4);
    ifs.read((char*) &header.num_rows, 4);
    ifs.read((char*) &header.num_cols, 4);

    if (is_little_endian()) {
        reverse_endian(&header.magic_number);
        reverse_endian(&header.num_items);
        reverse_endian(&header.num_rows);
        reverse_endian(&header.num_cols);
    }

    if (header.magic_number != 0x00000803 || header.num_items <= 0)
        std::cerr << "MNIST image-file format error" << std::endl;
    if (ifs.fail() || ifs.bad())
        std::cerr << "parse header error" << std::endl;


    std::cout << "magic number: " << std::showbase << std::hex 
        << header.magic_number << std::noshowbase << std::endl;
    std::cout << "num of items: " << std::dec << header.num_items << std::endl;
    std::cout << "num of rows : " << header.num_rows << std::endl;
    std::cout << "num of cols : " << header.num_cols << std::endl;
}

/*
 * 处理一张 mnist 图像 
 * 具体什么意思画个图就明白了, 原图大小是 (num_rows, num_cols),
 * 加上 padding 之后为 (num_rows + 2 * y_padding, num_cols + 2 * x_padding)
 * 然后将原图中的数据拷贝到 dst 中, 而 padding 中的数据使用 
 * dst.resize(height * width, scale_min) 设置为 scale_min
 */ 

inline void parse_mnist_image(std::ifstream &ifs,
                            const mnist_header &header,
                            std::vector<double_t> &dst,
                            double_t scale_min = 0.0,
                            double_t scale_max = 1.0,
                            int x_padding = 0,
                            int y_padding = 0) {
   if (x_padding < 0 || y_padding < 0)
       std::cerr << "padding size must not be negative" << std::endl;
   if (scale_max <= scale_min)
       std::cerr << "scale_max must be greater than scale_min" << std::endl;
   const int width = header.num_cols + 2 * x_padding;
   const int height = header.num_rows + 2 * y_padding;

   std::vector<uint8_t> image_vec(header.num_rows * header.num_cols);
   // 将图像数据读入 image_vec 中(注意是 uint8_t)
   // 下面必须写成 &image_vec[0] 而不能是 &image_vec, 后者是 vector 对象的地址
   ifs.read((char*) &image_vec[0], header.num_rows * header.num_cols);
   dst.resize(height * width, scale_min); // resize 为 vector 的方法
   for (uint32_t y = 0; y < header.num_rows; ++y)
       for (uint32_t x = 0; x < header.num_cols; ++x)
           dst[width * (y + y_padding) + x + x_padding]
            = (image_vec[y * header.num_cols + x] / double_t(255))
              * (scale_max - scale_min) + scale_min;
}

/*
 * 处理所有的 label
 */
inline void parse_mnist_labels(const std::string &label_file, 
                            std::vector<label_t> *labels) {
    std::ifstream ifs(label_file.c_str(), std::ios::in | std::ios::binary) ;
    if (ifs.fail() || ifs.bad())
        std::cerr << "failed to open file: " + label_file << std::endl;

    uint32_t magic_number, num_items;
    ifs.read((char*) &magic_number, 4);
    ifs.read((char*) &num_items, 4);

    if (is_little_endian()) {
        reverse_endian(&magic_number);
        reverse_endian(&num_items);
    }

    if (magic_number != 0x00000801 || num_items <= 0)
        std::cerr << "MNIST label-file format error!" << std::endl;

    for (uint32_t i = 0; i < num_items; ++i) {
        uint8_t label;
        ifs.read((char*) &label, 1);
        labels->push_back((label_t) label);
    }

    ifs.close();
}


int main() {
    const std::string image_file = data_dir_path + "/t10k-images.idx3-ubyte";
    const std::string label_file = data_dir_path + "/t10k-labels.idx1-ubyte";
    std::ifstream ifs(image_file.c_str(), std::ios::in | std::ios::binary);
    if (ifs.fail() || ifs.bad())
        std::cerr << "file error!" << std::endl;
    mnist_header header; // 保存数据集的 header 信息
    parse_mnist_header(ifs, header);
    std::vector<double_t> image; // 存储一张图的数据
    parse_mnist_image(ifs, header, image);

    std::vector<label_t> labels; // 存储所有的 labels
    parse_mnist_labels(label_file, &labels);
    std::cout << "label number: " << labels.size() << std::endl;
    std::cout << labels[0] << std::endl;

    // 这样设置之后, 全屏看效果更佳
    std::cout << std::setprecision(3);
    for (size_t i = 0; i < image.size(); ++i) {
        std::cout << std::setw(6) << image[i];
        if ((i + 1) % header.num_rows == 0)
            std::cout << std::endl;
    }


    return 0;
}
