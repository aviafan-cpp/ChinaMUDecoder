#include "train_data_ex.hpp"

#include <vector>
#include <string>
#include <filesystem>
#include <set>
#include <map>
#include <stdexcept>
#include <memory>
#include <utility>

#include <fstream>
#include <algorithm>
#include <sstream>

#define throw_or_continue(...)                                        \
    do {                                                              \
        if (throw_when_error)                                         \
            throw __VA_ARGS__;                                        \
        else                                                          \
            continue;                                                 \
    } while (0)

namespace train_data_ex
{
    // train_info_manager

    void train_info_manager::load(const std::string& path, bool throw_when_error)
    {
        this->load(std::filesystem::path(path), throw_when_error);
    }

    void train_info_manager::load(const std::filesystem::path& path, bool throw_when_error)
    {
        std::ifstream file;
        if (throw_when_error) file.exceptions(std::ifstream::badbit);
        file.open(path);

        std::string line;
        size_t line_index = 0;
        train_info* out = &this->self;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            line_index ++;

            if (line.empty())
            {
                out->info.emplace_back();
                continue;
            }

            switch (line[0])
            {
            case '+':
            {
                auto name = line.substr(1);
                if (name.empty()) throw_or_continue(file_format_error("'+' 命令的名称为空。", line_index, line));
                out = &this->trains[name];
                break;
            }
            
            case '*':
            {
                auto content = line.substr(1);
                if (content.empty()) throw_or_continue(file_format_error("'*' 命令的内容为空。", line_index, line));
                
                // 处理 "()" 内容
                std::set<std::string> temp_attribute; // 临时保存结果，因为此时无目标
                auto paren_begin = std::find(content.begin(), content.end(), '(');
                if (paren_begin != content.end())
                {
                    auto paren_end = std::find(paren_begin, content.end(), ')');
                    if (paren_end == content.end()) throw_or_continue(file_format_error("'*' 命令的括号不匹配。", line_index, line));
                    if (paren_begin != content.begin()) throw_or_continue(file_format_error("属性必须出现在 '*' 内容开头。", line_index, line));

                    auto attributes = content.substr(paren_begin - content.begin() + 1, paren_end - paren_begin - 1);
                    if (attributes.empty()) throw_or_continue(file_format_error("'*' 命令的括号内无内容。", line_index, line));
                    content.erase(paren_begin - content.begin(), paren_end - content.begin() + 1);

                    // 按 ',' 切块
                    std::istringstream iss(attributes);
                    std::string token;
                    while (std::getline(iss, token, ',')) temp_attribute.emplace(token);
                }

                // 处理 '='
                if (content.size() < 3) throw_or_continue(file_format_error("'*' 缺失部分内容。", line_index, line)); // 名称、'='、内容每个至少1
                auto equal_sign = std::find(content.begin(), content.end(), '=');
                if (equal_sign == content.end()) throw_or_continue(file_format_error("'*' 没有对应的 '=' 。", line_index, line));
                auto out_data_name = content.substr(0, equal_sign - content.begin());
                out->datas[out_data_name].value = content.substr(equal_sign - content.begin() + 1);
                out->datas[out_data_name].attribute = temp_attribute;

                break;
            }

            case '#':
                continue;
            
            default:
                out->info.emplace_back(line);
                break;
            }
        }
    }
}
