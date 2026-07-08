#include "train_data.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace train_data
{
    void train_model_manager::add_train_model(train_model add_tm)
    {
        trains.push_back(add_tm);
    }

    void train_model_manager::add_train_model_from_file(std::string path)
    {
        std::ifstream file(path);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line))
        {
            if (line.empty()) continue; // 跳过空行
            if(line[0] == '#') continue; // 屏蔽数据库信息

            // Lambda 函数：将字符串按空格拆分成指定段数，返回 vector<string>
            auto split_to_n = [](const std::string& str, size_t n) -> std::vector<std::string>
            {
                std::vector<std::string> result(n);  // 预分配 n 个位置，默认空字符串
                std::istringstream iss(str);
                
                std::string word;
                size_t i = 0;
                while (iss >> word && i < n) {
                    result[i++] = word;
                }
                
                return result;
            };

            // ---- 处理删除命令 ----
            if (line[0] == '-')
            {
                std::string sub_line = line.substr(1);
                std::vector<std::string> parts = split_to_n(sub_line, 3);

                std::string train_name;
                unsigned int left, right;
                try
                {
                    train_name = parts[0];
                    left = std::stoi(parts[1]);
                    right = std::stoi(parts[2]);
                }
                catch (const std::exception& e)
                {
                    continue;
                }

                remove_subrange(train_name, left, right);
                continue;
            }

            std::vector<std::string> parts = split_to_n(line, 7);
            for(int i = 0; i < parts.size(); ++i) if(parts[i] == "null") parts[i] = "";
            train_model new_tm;

            try
            {
                new_tm.name = parts[0];
                new_tm.max_speed = std::stoi(parts[1]);
                new_tm.cars_number = std::stoi(parts[2]);
                new_tm.train_type = parts[3];
                new_tm.plate_number_start = std::stoi(parts[4]);
                new_tm.plate_number_end = std::stoi(parts[5]);
                new_tm.science_website_url = parts[6];
            }
            catch(const std::exception& e)
            {
                continue;
            }
            
            trains.emplace_back(new_tm);
        }
        
    }

    void train_model_manager::remove_subrange(const std::string& train_name, unsigned int left, unsigned int right)
    {
        if (left > right) std::swap(left, right);

        size_t original_size = trains.size();
        for (size_t i = 0; i < original_size; ++i)
        {
            train_model& tm = trains[i];

            // 检查型号名称是否匹配，以及区间是否有交集
            if (tm.name != train_name) continue;
            if (tm.plate_number_end < left || tm.plate_number_start > right) continue;

            // 保存原始区间边界
            unsigned int orig_start = tm.plate_number_start;
            unsigned int orig_end = tm.plate_number_end;

            // ---- 情况1：删除左半边 [orig_start, right] ----
            if (left <= orig_start && right < orig_end)
            {
                tm.plate_number_start = right + 1;
            }
            // ---- 情况2：删除右半边 [left, orig_end] ----
            else if (left > orig_start && right >= orig_end)
            {
                tm.plate_number_end = left - 1;
            }
            // ---- 情况3：删除中间 [left, right]（原区间被拆成两段） ----
            else if (left > orig_start && right < orig_end)
            {
                // 修改原区间为 [orig_start, left - 1]
                tm.plate_number_end = left - 1;

                // 创建新区间 [right + 1, orig_end]
                train_model new_tm = tm;
                new_tm.plate_number_start = right + 1;
                new_tm.plate_number_end = orig_end;
                trains.push_back(new_tm);
            }
            // ---- 情况4：完全删除整个区间 [orig_start, orig_end] ----
            else if (left <= orig_start && right >= orig_end)
            {
                // 标记为空区间（start > end）
                tm.plate_number_start = 1;
                tm.plate_number_end = 0;
            }
        }

        // 移除所有被标记为空区间的条目
        for (int i = static_cast<int>(trains.size()) - 1; i >= 0; --i)
        {
            if (trains[i].plate_number_start > trains[i].plate_number_end)
            {
                trains.erase(trains.begin() + i);
            }
        }
    }

    std::vector<train_model> train_model_manager::find_train_model(unsigned int plate_number) const
    {
        std::vector<train_model> find_tms;
        for(size_t i = 0; i < trains.size(); ++i)
        {
            if(plate_number >= trains[i].plate_number_start && plate_number <= trains[i].plate_number_end)
            {
                find_tms.emplace_back(trains[i]);
            }
        }
        return find_tms;
    }

    std::vector<std::string> train_model_manager::get_train_type_list() const
    {
        std::vector<std::string> result;
        for(size_t i = 0; i < trains.size(); ++i)
        {
            auto it = std::find(result.begin(), result.end(), trains[i].train_type);
            if(it == result.end()) result.push_back(trains[i].train_type);
        }
        return result;
    }

    std::vector<train_model> train_model_manager::find_train_type(std::string train_type) const
    {
        std::vector<train_model> find_tms;
        for(size_t i = 0; i < trains.size(); ++i)
        {
            if(trains[i].train_type == train_type) find_tms.push_back(trains[i]);
        }
        return find_tms;
    }
}
