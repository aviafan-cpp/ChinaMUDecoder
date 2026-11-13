#ifndef TRAIN_DATA_EX
#define TRAIN_DATA_EX

#include <vector>
#include <string>
#include <filesystem>
#include <set>
#include <map>
#include <stdexcept>
#include <memory>
#include <utility>

namespace train_data_ex
{
    // 文件解码错误
    struct file_format_error : public std::runtime_error {
    private:
        size_t index_;
        std::shared_ptr<const std::string> content_;
        
    public:
        file_format_error(const std::string& msg, size_t index, std::string content) : std::runtime_error(msg), index_(index), content_(std::make_shared<const std::string>(std::move(content))) {}
        
        size_t index() const noexcept { return index_; }
        const std::string& content() const noexcept { return *content_; }
    };

    struct data_with_attribute
    {
        std::string value;
        std::set<std::string> attribute;
    };
    
    struct train_info
    {
        std::vector<std::string> info;
        std::map<std::string, data_with_attribute> datas;
    };
    
    struct train_info_manager
    {
        train_info self; // 数据库自身的信息
        std::map<std::string, train_info> trains;

        void load(const std::string& path, bool throw_when_error = true);
        void load(const std::filesystem::path& path, bool throw_when_error = true);
    };
    
}

#endif
