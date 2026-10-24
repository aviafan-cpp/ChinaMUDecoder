#include <string>
#include <filesystem>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>
#include <iterator>
#include <exception>
#include "raylib.h"
#include "raylib_obj_ex.hpp"
#include "train_data.hpp"
#include "sys_api.hpp"

using namespace raylib_obj_ex::coord;
using namespace raylib_obj_ex::text_ns;
using namespace raylib_obj_ex::button_ns;
using raylib_obj_ex::horizontal;

const int design_x = 720; // 设计分辨率宽度
const int design_y = 480; // 设计分辨率高度
const int design_fps = 30; // 设计帧率
std::string current_dir; // 当前目录

const std::string program_name = "中国动车组车号查询";
const unsigned int program_version = 20261018u;

enum screen {filter_plate_number, about, choose_list_type, train_name_list, filter_train_name, train_type_list, filter_train_type}; // 查询车号（主界面）、筛选列车命名
screen g_screen = filter_plate_number; // 界面管理

train_data::train_model_manager g_train; // 车辆数据管理器
std::string plate_number; // 车号
std::vector<train_data::train_model> temp_train_data; // 临时数据缓存
std::string temp_plate_number; // 临时缓存车号

std::map<std::string, std::string> public_data; // 软件全局变量

std::vector<std::string> data_file_info; // 数据文件的信息

Font middle_font;
Font small_font;
Texture2D main_menu_train_picture; // 主菜单火车的图片

// 函数定义，实现在文件末尾
int show_choose_list(coord_calc menu_x, coord_calc menu_y, float position_y, size_t page_number, button middle_button, std::vector<std::string> content); // 批量显示并返回点击

int main(int argc, const char* argv[])
{
    auto get_dir = [](const std::string& path) -> std::string {
        std::filesystem::path p(path);
        std::filesystem::path parent = p.parent_path();
        
        if (parent.empty()) {
            return "";
        }
        return parent.string();
    };
    current_dir = get_dir(argv[0]);

    std::string data_path = current_dir + "/Data/TrainData.txt";
    g_train.add_train_model_from_file(data_path);
    auto get_data_file_info = [](std::string path, char mark_char) -> std::vector<std::string>
    {
        std::vector<std::string> back_str;
        std::fstream file(path);
        if(!file.is_open()) return {};
        std::string line;
        while (std::getline(file, line))
        {
            if(line.size() > 1)
                if(line[0] == mark_char) back_str.push_back(line.substr(1));
        }
        return back_str;
    };
    data_file_info = get_data_file_info(data_path, '#');

    InitWindow(design_x, design_y, program_name.c_str());
    SetWindowState(FLAG_WINDOW_RESIZABLE); // 允许用户自由调整窗口大小
    SetWindowMinSize(design_x, design_y); // 设置最小窗口大小
    SetTargetFPS(design_fps);

    coord_calc g_menu_x, g_menu_y; // 自动缩放系统
    g_menu_x.design_resolution = design_x;
    g_menu_y.design_resolution = design_y;

    middle_font = raylib_obj_ex::font::load_font_chinese(current_dir + "/Font/LXGWWenKai-Regular.ttf", 80, true, false, false, true);
    main_menu_train_picture = LoadTexture((current_dir + "/Picture/1080_1080.jpg").c_str());

    try
    {
        if(argc > 1) plate_number = argv[1];
    }
    catch(const std::exception& e){}

    // 定义UI组件“模版”
    text middle_text; // 普通文字
    text big_text; // 大号文字（标题）
    button middle_button; // 普通按钮

    while (!WindowShouldClose())
    {
        // 输入处理
        // 1.数字
        int numberKeys[] = {KEY_ZERO, KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE, KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE};
        for (int i = 0; i < 10; i++)
        {
            if (IsKeyPressed(numberKeys[i]) && plate_number.size() < 4)
            {
                plate_number.push_back('0' + i);
            }
        }
        // 2.特殊按键
        if(IsKeyPressed(KEY_BACKSPACE) && !plate_number.empty()) plate_number.pop_back();
        if(IsKeyPressed(KEY_ENTER))
        {
            while(plate_number.size() < 4) plate_number = '0' + plate_number;
        }

        // 获取窗口分辨率
        g_menu_x.runtime_resolution = GetScreenWidth();
        g_menu_y.runtime_resolution = GetScreenHeight();

        // 处理UI组件“模版”
        middle_text.set(middle_font, "", {0, 0}, std::min(g_menu_x.calc(30), g_menu_y.calc(30)), g_menu_x.calc(5), BLUE, raylib_obj_ex::Center);
        big_text.set(middle_font, "", {0, 0}, std::min(g_menu_x.calc(40), g_menu_y.calc(40)), g_menu_x.calc(5), BLUE, raylib_obj_ex::Center);
        middle_button.set({0, 0}, {g_menu_x.calc(80), g_menu_y.calc(30)}, SKYBLUE, middle_text);

        BeginDrawing();
        ClearBackground(WHITE);

        text(big_text).set(program_name, {g_menu_x.calc(0.5f), 0}).draw();
        if(g_screen == filter_plate_number)
        {
            if(plate_number.empty())
            button(middle_button).set({g_menu_x.calc(0.15f), g_menu_y.calc(5)}, "请输入车号……")._tint(WHITE)._text_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))._text_spacing(0)._size_width(g_menu_y.calc(120)).draw();
        else 
            button(middle_button).set({g_menu_x.calc(0.15f), g_menu_y.calc(5)}, plate_number)._tint(WHITE)._text_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))._text_spacing(0).draw();
        }
        if(g_screen == filter_plate_number && plate_number.size() == 0)
        {
            if(button(middle_button).set({g_menu_x.calc(0.85f), g_menu_y.calc(5)}, "退出")._tint(WHITE).draw().is_clicked()) break;
        }
        else
        {
            if(button(middle_button).set({g_menu_x.calc(0.85f), g_menu_y.calc(5)}, "主菜单")._size_width(g_menu_x.calc(100))._tint(WHITE).draw().is_clicked())
            {
                g_screen = filter_plate_number;
                plate_number = "";
            }
        }
        DrawLine(0, g_menu_y.calc(40), g_menu_x.runtime_resolution, g_menu_y.calc(40), BLUE);

        switch (g_screen)
        {

            case filter_plate_number:
            {
                // 查询界面
                if(plate_number.size() < 4)
                {
                    // 主界面

                    // 主图片
                    DrawTextureEx(main_menu_train_picture, {g_menu_x.calc(10), g_menu_y.calc(50)}, 0, std::min(float(g_menu_x.runtime_resolution) / 1350, float(g_menu_y.runtime_resolution) / 1350), WHITE);

                    // 教程与数据库信息
                    text(middle_text)
                        .set("输入车辆的四位数车号查询。", {g_menu_x.calc(0.575f), g_menu_y.calc(60)})
                        ._font_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                        ._spacing(2)
                        ._horizontal(horizontal::Left)
                        .draw();
                    text(middle_text)
                        .set("版本：" + std::to_string(program_version), {g_menu_x.calc(0.575f), g_menu_y.calc(80)})
                        ._font_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                        ._spacing(2)
                        ._horizontal(horizontal::Left)
                        .draw();
                    text(middle_text)
                        .set("程序位数：" + std::to_string(sizeof(void*) * 8) + "位", {g_menu_x.calc(0.575f), g_menu_y.calc(100)})
                        ._font_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                        ._spacing(2)
                        ._horizontal(horizontal::Left)
                        .draw();
                    text(middle_text)
                        .set("已加载数据量：" + std::to_string(g_train.get_train_name_list().size()) + "款车辆数据", {g_menu_x.calc(0.575f), g_menu_y.calc(120)})
                        ._font_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                        ._spacing(2)
                        ._horizontal(horizontal::Left)
                        .draw();
                    text(middle_text)
                        .set("以下为数据库信息：", {g_menu_x.calc(0.575f), g_menu_y.calc(140)})
                        ._font_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                        ._spacing(2)
                        ._horizontal(horizontal::Left)
                        .draw();
                    for(size_t i = 0; i < data_file_info.size(); ++i)
                    {
                        text(middle_text)
                            .set(data_file_info[i], {g_menu_x.calc(0.575f), g_menu_y.calc(int(160 + 20 * i))})
                            ._font_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                            ._spacing(2)
                            ._horizontal(horizontal::Left)
                            .draw();
                    }

                    // 筛选
                    if(button(middle_button)
                        .set({float(g_menu_x.runtime_resolution) / 4, g_menu_y.calc(455)}, "筛选")
                        ._horizontal(raylib_obj_ex::Center)
                        ._size({g_menu_x.calc(125), g_menu_y.calc(20)})
                        ._text_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                        ._tint(WHITE)
                        .draw()
                        .is_clicked()
                    )
                    {
                        g_screen = choose_list_type;
                    }
                    // 版权与引用声明
                    text(middle_text).set("Copyright (c) 2026 aviafan-cpp https://github.com/aviafan-cpp", {float(g_menu_x.runtime_resolution) / 2, g_menu_y.calc(445)})._spacing(3)._horizontal(raylib_obj_ex::Center)._font_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20))).draw();
                    if(button(middle_button)
                        .set({float(g_menu_x.runtime_resolution) / 2, g_menu_y.calc(455)}, "作者及引用库")
                        ._horizontal(raylib_obj_ex::Center)
                        ._size({g_menu_x.calc(150), g_menu_y.calc(20)})
                        ._text_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                        ._tint(WHITE)
                        .draw()
                        .is_clicked()
                    )
                    {
                        g_screen = about;
                    }
                    // 置顶按钮
                    {
                        std::string content;
                        if(IsWindowState(FLAG_WINDOW_TOPMOST))
                            content = "取消置顶";
                        else
                            content = "置顶";

                        if(button(middle_button)
                            .set({float(g_menu_x.runtime_resolution) / 4 * 3, g_menu_y.calc(455)}, content)
                            ._horizontal(raylib_obj_ex::Center)
                            ._size({g_menu_x.calc(125), g_menu_y.calc(20)})
                            ._text_size(std::min(g_menu_x.calc(20), g_menu_y.calc(20)))
                            ._tint(WHITE)
                            .draw()
                            .is_clicked()
                        )
                        {
                            if(!IsWindowState(FLAG_WINDOW_TOPMOST))
                                SetWindowState(FLAG_WINDOW_TOPMOST);
                            else
                                ClearWindowState(FLAG_WINDOW_TOPMOST);
                        }
                    }
                }
                else if(plate_number.size() == 4)
                {
                    // 查询界面

                    if(plate_number != temp_plate_number) 
                    {
                        temp_train_data = g_train.find_train_model(std::stoi(plate_number));
                        temp_plate_number = plate_number;
                    }

                    for(size_t i = 0; i < std::min(temp_train_data.size(), size_t(8)); ++i)
                    {
                        float y = g_menu_y.calc(int(50 * (i + 1)));
                        std::string t =
                            temp_train_data[i].name + " "+
                            temp_train_data[i].train_type + " " +
                            std::to_string(temp_train_data[i].max_speed) + "km/h " +
                            std::to_string(temp_train_data[i].cars_number) + "车厢 ";
                        Color text_tint = temp_train_data[i].train_type.find("复兴号") != std::string::npos ? GOLD : BLUE ;
                        bool ic = button(middle_button)
                            ._horizontal(raylib_obj_ex::Left)
                            .set({g_menu_x.calc(10), y}, t)
                            ._tint(WHITE)
                            ._text_spacing(0)
                            ._size_width(g_menu_x.calc(700))
                            ._text_tint(text_tint)
                            .draw()
                            .is_clicked();
                        if(ic)
                        {
                            if(!temp_train_data[i].science_website_url.empty()) sys_api::open_url(temp_train_data[i].science_website_url);
                            sys_api::open_url("https://rail.re/#" + temp_train_data[i].name + "-" + plate_number);
                        }

                        float text_y = g_menu_y.calc(int(50 * (i + 1) + 40));
                        text(middle_text).set(temp_train_data[i].science_website_url, {g_menu_x.calc(710), text_y})._horizontal(raylib_obj_ex::Right)._font_size(std::min(g_menu_x.calc(15), g_menu_y.calc(15)))._spacing(3).draw();
                    }

                    text(middle_text).set("点击列车名称启动对应的科普网站及使用rail.re查询交路。动力集中动车组车厢数考虑机车。", {g_menu_x.calc(10), g_menu_y.calc(470)})._tint(RED)._font_size(std::min(g_menu_x.calc(15), g_menu_y.calc(15))).draw();
                }
                else plate_number = "";

                break;
            }

            case about:
            {
                struct name_and_website_url{
                    std::string name;
                    std::string website_url;
                };
                std::vector<name_and_website_url> names{
                    {"作者：aviafan_cpp", ""},
                    {"-Github", "https://github.com/aviafan-cpp"},
                    {"-Bilibili", "https://space.bilibili.com/3546620213857006"},
                    {"-Moegirlpedia", "https://zh.moegirl.org.cn/User:AviaFanCpp"},
                    {"图形库：Raylib", ""},
                    {"-Website", "https://raylib.com"},
                    {"-Github", "https://github.com/raysan5/raylib"},
                    {"字体：霞鹜文楷", ""},
                    {"-Github", "https://github.com/lxgw/LxgwWenKai"},
                    {"项目网址：", ""},
                    {"-Github", "https://github.com/aviafan-cpp/ChinaMUDecoder"},
                    {"-Github Releases", "https://github.com/aviafan-cpp/ChinaMUDecoder/releases"}
                };

                std::vector<std::string> show;
                std::transform(names.begin(), names.end(), std::back_inserter(show), [](const name_and_website_url& nwu) { return nwu.name; });
                int result = show_choose_list(g_menu_x, g_menu_y, 50, 8, middle_button, show);

                if(result >= 0 && result < names.size() && !names[result].website_url.empty())
                {
                    sys_api::open_url(names[result].website_url);
                }

                break;
            }

            case choose_list_type:
            {
                struct page{
                    std::string name;
                    screen index;
                };
                std::vector<page> filter_types{
                    {"筛选车型名称", train_name_list},
                    {"筛选命名", train_type_list}
                };

                std::vector<std::string> show;
                std::transform(filter_types.begin(), filter_types.end(), std::back_inserter(show), [](const page& p) { return p.name; });
                int result = show_choose_list(g_menu_x, g_menu_y, 50, 8, middle_button, show);

                if(result >= 0 && result < filter_types.size())
                {
                    g_screen = filter_types[result].index;
                }

                break;
            }

            case train_name_list:
            {
                // 筛选列车车型名称
                auto train_names = g_train.get_train_name_list();
                int result = show_choose_list(g_menu_x, g_menu_y, 50, 8, middle_button, train_names);
                if(result >= 0)
                {
                    public_data["filter_train_name.train_name"] = train_names[result];
                    g_screen = filter_train_name;
                }
                break;
            }

            case filter_train_name:
            {
                // 查找列车车型名称
                auto same_type_train = g_train.filter_train_name(public_data["filter_train_name.train_name"]);
                std::vector<std::string> show_content;
                for(auto& astt : same_type_train)
                    show_content.emplace_back(astt.to_string());
                int result = show_choose_list(g_menu_x, g_menu_y, 50, 8, middle_button, show_content);
                if(result >= 0 && !same_type_train[result].science_website_url.empty()) sys_api::open_url(same_type_train[result].science_website_url);
                break;
            }

            case train_type_list:
            {
                // 筛选列车命名
                auto train_types = g_train.get_train_type_list();
                int result = show_choose_list(g_menu_x, g_menu_y, 50, 8, middle_button, train_types);
                if(result >= 0)
                {
                    public_data["filter_train_type.train_type"] = train_types[result];
                    g_screen = filter_train_type;
                }
                break;
            }

            case filter_train_type:
            {
                // 查找列车命名
                auto same_type_train = g_train.filter_train_type(public_data["filter_train_type.train_type"]);
                std::vector<std::string> show_content;
                for(auto& astt : same_type_train)
                    show_content.emplace_back(astt.to_string());
                int result = show_choose_list(g_menu_x, g_menu_y, 50, 8, middle_button, show_content);
                if(result >= 0 && !same_type_train[result].science_website_url.empty()) sys_api::open_url(same_type_train[result].science_website_url);
                break;
            }

            default:
            {
                g_screen = filter_plate_number;
            }

        }

        EndDrawing();
    }
    
    UnloadFont(middle_font);
    UnloadFont(small_font);
    UnloadTexture(main_menu_train_picture);

    return 0;
}

int show_choose_list(coord_calc menu_x, coord_calc menu_y, float position_y, size_t page_number, button middle_button, std::vector<std::string> content)
{
    static size_t current_page; // 静态变量记录当前页数
    static std::vector<std::string> last_content; // 静态变量记录上一次传入内容，不同时重置current_page
    if(last_content != content)
    {
        current_page = 0;
        last_content = content;
    }

    int result = -1;
    
    // 计算总页数
    size_t total_pages = (content.size() + page_number - 1) / page_number;
    if (total_pages == 0) total_pages = 1;
    
    // 确保当前页在有效范围内
    if (current_page >= total_pages) current_page = total_pages - 1;
    
    // 计算当前页的起始和结束索引
    size_t start_index = current_page * page_number;
    size_t end_index = std::min(start_index + page_number, content.size());
    
    // 显示当前页的内容
    for(size_t i = start_index; i < end_index; ++i)
    {
        float y = menu_y.calc(int(position_y * (i - start_index + 1)));
        bool ic = button(middle_button)
                    ._horizontal(raylib_obj_ex::Left)
                    .set({menu_x.calc(10), y}, content[i])
                    ._tint(WHITE)
                    ._text_spacing(0)
                    ._size_width(menu_x.calc(700))
                    .draw()
                    .is_clicked();
        if(ic) result = i;
    }
    
    // 原计划翻页按钮放在 g_menu_y.calc(450) 的位置
    float button_y = menu_y.calc(int(position_y * (end_index - start_index + 1)));
    float button_width = menu_x.calc(60);
    float button_height = menu_y.calc(20);
    
    // 上一页按钮
    if(current_page > 0)
    {
        if(button(middle_button)
            .set({menu_x.calc(50), button_y}, "上一页")
            ._size({button_width, button_height})
            ._text_size(std::min(menu_x.calc(15), menu_y.calc(15)))
            ._tint(WHITE)
            .draw()
            .is_clicked())
        {
            current_page--;
        }
    }
    
    // 下一页按钮
    if(current_page < total_pages - 1)
    {
        if(button(middle_button)
            .set({menu_x.runtime_resolution - menu_x.calc(50), button_y}, "下一页")
            ._size({button_width, button_height})
            ._text_size(std::min(menu_x.calc(15), menu_y.calc(15)))
            ._tint(WHITE)
            .draw()
            .is_clicked())
        {
            current_page++;
        }
    }
    
    return result;
}