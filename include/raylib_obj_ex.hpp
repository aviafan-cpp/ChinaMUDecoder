#ifndef RAYLIB_OBJ_EX
#define RAYLIB_OBJ_EX

#include "raylib.h"
#include <string>

namespace raylib_obj_ex
{
    enum horizontal {Left, Center, Right}; // 渲染对齐方向

    namespace coord
    {
        struct coord_calc
        {
            int runtime_resolution;   // 实际运行的屏幕尺寸
            int design_resolution;    // 设计时的基准屏幕尺寸
            
            float calc(int design_length); // 传入设计时位置
            float calc(float design_percentage); // 传入设计时百分比
        };
    }

    namespace text_ns
    {
        struct text
        {
            Font font; // 字体
            std::string st; // 文本
            Vector2 position; // 坐标
            float font_size; // 字体大小
            float spacing; // 字体间隔
            Color tint; // 字体颜色
            horizontal current_horizontal; // 渲染对齐方向

            // 链式设置函数
            text& _font(Font _font);
            text& _st(std::string _st);
            text& _position(Vector2 _position);
            text& _position_x(float _x);
            text& _position_y(float _y);
            text& _font_size(float _font_size);
            text& _spacing(float _spacing);
            text& _tint(Color _tint);
            text& _horizontal(horizontal _current_horizontal);

            text() : font(GetFontDefault()), font_size(20), spacing(1), tint(BLACK), current_horizontal(Left), position{0, 0} {}
            text& copy(text other);
            text& set(Font font, std::string st, Vector2 position, float font_size, float spacing, Color tint, horizontal current_horizontal);
            text& set(std::string st, Vector2 position, float font_size, float spacing, Color tint, horizontal current_horizontal);
            text& set(std::string st, Vector2 position);
            text& change(std::string st, Vector2 offset);

            text& draw();
        };
        
    }

    namespace button_ns
    {
        struct button
        {
            Vector2 position;
            Vector2 size;
            Color tint;
            Font font;
            std::string text;
            float text_size;
            float text_spacing;
            Color text_tint;
            bool is_pressed; // 按下
            horizontal current_horizontal; // 渲染对齐方向

            // 链式设置函数
            button& _position(Vector2 _position);
            button& _position_x(float _x);
            button& _position_y(float _y);
            button& _size(Vector2 _size);
            button& _size_width(float _width);
            button& _size_height(float _height);
            button& _tint(Color _tint);
            button& _font(Font _font);
            button& _text(std::string _text);
            button& _text_size(float _text_size);
            button& _text_spacing(float _text_spacing);
            button& _text_tint(Color _text_tint);
            button& _pressed(bool _is_pressed);
            button& _horizontal(horizontal _current_horizontal);

            button() : position{0, 0}, size{100, 40}, tint(LIGHTGRAY), font(GetFontDefault()), text_size(20), text_spacing(1), text_tint(BLACK), is_pressed(false), current_horizontal(Left) {}
            button& copy(button other);
            button& set(Vector2 position, Vector2 size, Color tint, Font font, std::string text, float text_size, float text_spacing, Color text_tint, bool is_pressed, horizontal current_horizontal);
            button& set(Vector2 position, Vector2 size, Color tint, text_ns::text text);
            button& set(Vector2 position, std::string text);
            button& change(Vector2 offset, std::string text);

            button& draw();

            bool is_clicked();
        };
    }

    namespace font
    {
        Font load_font_chinese(std::string fileName, int fontSize, bool chinese, bool chinese_ex_a, bool chinese_ex_b, bool chinese_symbol);
    }
}

#endif