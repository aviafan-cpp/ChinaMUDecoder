#ifndef TRAIN_DATA
#define TRAIN_DATA

#include <string>
#include <vector>

namespace train_data
{
    struct train_model
    {
        std::string name;
        unsigned int max_speed;
        unsigned int cars_number;
        std::string train_type;

        unsigned int plate_number_start; // 起始车号
        unsigned int plate_number_end; // 末尾车号

        std::string science_website_url; // 引用的科普软件链接
    };

    struct train_model_manager
    {
        std::vector<train_model> trains;

        void add_train_model(train_model add_tm);
        void add_train_model_from_file(std::string path);

        void remove_subrange(const std::string& train_name, unsigned int left, unsigned int right); // 处理删除命令：从匹配的型号区间中移除指定子区间

        std::vector<train_model> find_train_model(unsigned int plate_number) const; // 获取所有包含该车号的型号
    };
}

#endif