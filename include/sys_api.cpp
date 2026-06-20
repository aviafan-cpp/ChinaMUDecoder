#include "sys_api.hpp"
#include <string>

namespace sys_api
{
    #if defined(_WIN32) || defined(_WIN64)
        #include <windows.h>
        std::wstring stow(const std::string& str, UINT code_page = CP_UTF8) {
            // 1. 参数校验
            if (str.empty()) {
                return L"";
            }
        
            // 2. 计算转换后宽字符串所需的长度（包含结尾的'\0'）
            int requiredLen = MultiByteToWideChar(code_page, 0, str.c_str(), -1, nullptr, 0);
            if (requiredLen == 0) {
                // 转换失败，可根据需要抛出异常或返回空字符串
                return L"";
            }
        
            // 3. 为 std::wstring 分配所需空间
            std::wstring wstr(requiredLen, L'\0');
        
            // 4. 执行实际的编码转换
            int convertedLen = MultiByteToWideChar(code_page, 0, str.c_str(), -1, &wstr[0], requiredLen);
            if (convertedLen == 0) {
                return L"";
            }
        
            // 5. 移除末尾的 '\0' 字符
            wstr.pop_back(); 
            
            return wstr;
        }

        void open_url(std::string url)
        {
            std::wstring wurl = stow(url);
            ShellExecuteW(NULL, L"open", wurl.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }

    #endif
}