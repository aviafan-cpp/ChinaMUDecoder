#include "raylib_obj_ex.hpp"
#include "raylib.h"
#include <string>
#include <vector>

namespace raylib_obj_ex
{
    namespace coord
    {
        // coord_calc
        float coord_calc::calc(int design_length)
        {
            return design_length * runtime_resolution / design_resolution;
        }

        float coord_calc::calc(float design_percentage)
        {
            return design_percentage * runtime_resolution;
        }
    }

    namespace text_ns
    {
        // text_t
        text& text::_font(Font _font) { this->font = _font; return *this; }
        text& text::_st(std::string _st) { this->st = _st; return *this; }
        text& text::_position(Vector2 _position) { this->position = _position; return *this; }
        text& text::_position_x(float _x) { this->position.x = _x; return *this; }
        text& text::_position_y(float _y) { this->position.y = _y; return *this; }
        text& text::_font_size(float _font_size) { this->font_size = _font_size; return *this; }
        text& text::_spacing(float _spacing) { this->spacing = _spacing; return *this; }
        text& text::_tint(Color _tint) { this->tint = _tint; return *this; }
        text& text::_horizontal(horizontal _current_horizontal) { this->current_horizontal = _current_horizontal; return *this; }
        
        text& text::copy(text other)
        {
            *this = other;
            return *this;
        }

        text& text::set(Font font, std::string st, Vector2 position, float font_size, float spacing, Color tint, horizontal current_horizontal)
        {
            this->font = font;
            this->st = st;
            this->position = position;
            this->font_size = font_size;
            this->spacing = spacing;
            this->tint = tint;
            this->current_horizontal = current_horizontal;
            return *this;
        }

        text& text::set(std::string st, Vector2 position, float font_size, float spacing, Color tint, horizontal current_horizontal)
        {
            this->st = st;
            this->position = position;
            this->font_size = font_size;
            this->spacing = spacing;
            this->tint = tint;
            this->current_horizontal = current_horizontal;
            return *this;
        }

        text& text::set(std::string st, Vector2 position)
        {
            this->st = st;
            this->position = position;
            return *this;
        }

        text& text::change(std::string st, Vector2 offset)
        {
            this->st = st;
            this->position.x += offset.x;
            this->position.y += offset.y;
            return *this;
        }

        text& text::draw()
        {
            Vector2 text_size = MeasureTextEx(this->font, this->st.c_str(), this->font_size, this->spacing);
            Vector2 draw_pos = this->position;  // 使用临时变量
            
            switch (current_horizontal)
            {
            case Left:
                draw_pos.y -= text_size.y / 2;
                draw_pos.y = std::max(draw_pos.y, 0.0f);
                break;
            case Center:
                draw_pos.x -= text_size.x / 2;
                draw_pos.x = std::max(draw_pos.x, 0.0f);
                draw_pos.y -= text_size.y / 2;
                draw_pos.y = std::max(draw_pos.y, 0.0f);
                break;
            case Right:
                draw_pos.x -= text_size.x;
                draw_pos.x = std::max(draw_pos.x, 0.0f);
                draw_pos.y -= text_size.y / 2;
                draw_pos.y = std::max(draw_pos.y, 0.0f);
                break;
            }
            
            DrawTextEx(this->font, this->st.c_str(), draw_pos, this->font_size, this->spacing, this->tint);
            return *this;
        }
    }
    
    namespace button_ns
    {
        //button_t
        button& button::_position(Vector2 _position) { this->position = _position; return *this; }
        button& button::_position_x(float _x) { this->position.x = _x; return *this; }
        button& button::_position_y(float _y) { this->position.y = _y; return *this; }
        button& button::_size(Vector2 _size) { this->size = _size; return *this; }
        button& button::_size_width(float _width) { this->size.x = _width; return *this; }
        button& button::_size_height(float _height) { this->size.y = _height; return *this; }
        button& button::_tint(Color _tint) { this->tint = _tint; return *this; }
        button& button::_font(Font _font) { this->font = _font; return *this; }
        button& button::_text(std::string _text) { this->text = _text; return *this; }
        button& button::_text_size(float _text_size) { this->text_size = _text_size; return *this; }
        button& button::_text_spacing(float _text_spacing) { this->text_spacing = _text_spacing; return *this; }
        button& button::_text_tint(Color _text_tint) { this->text_tint = _text_tint; return *this; }
        button& button::_pressed(bool _is_pressed) { this->is_pressed = _is_pressed; return *this; }
        button& button::_horizontal(horizontal _current_horizontal) { this->current_horizontal = _current_horizontal; return *this; }

        button& button::copy(button other)
        {
            *this = other;
            return *this;
        }

        button& button::set(Vector2 position, Vector2 size, Color tint, Font font, std::string text, float text_size, float text_spacing, Color text_tint, bool is_pressed, horizontal current_horizontal)
        {
            this->position = position;
            this->size = size;
            this->tint = tint;
            this->font = font;
            this->text = text;
            this->text_size = text_size;
            this->text_spacing = text_spacing;
            this->text_tint = text_tint;
            this->is_pressed = is_pressed;
            this->current_horizontal = current_horizontal;
            return *this;
        }

        button& button::set(Vector2 position, Vector2 size, Color tint, text_ns::text _text)
    {
        this->position = position;
        this->size = size;
        this->tint = tint;
        this->font = _text.font;
        this->text = _text.st;
        this->text_size = _text.font_size;
        this->text_spacing = _text.spacing;
        this->text_tint = _text.tint;
        this->current_horizontal = _text.current_horizontal;
        this->is_pressed = false;
        return *this;
    }

        button& button::set(Vector2 position, std::string text)
        {
            this->position = position;
            this->text = text;
            this->is_pressed = false;
            return *this;
        }

        button& button::change(Vector2 offset, std::string text)
        {
            this->position.x += offset.x;
            this->position.y += offset.y;
            this->text = text;
            this->is_pressed = false;
            return *this;
        }

        button& button::draw()
        {
            this->is_pressed = false; // 在绘制时重置上一次点击状态

            Vector2 text_size = MeasureTextEx(this->font, this->text.c_str(), this->text_size, this->text_spacing);
            Vector2 draw_pos = this->position;
            
            // 计算按钮实际绘制位置（根据对齐方式偏移）
            Vector2 button_draw_pos = this->position;
            
            switch (current_horizontal)
            {
            case Left:
                // 按钮从position开始向右延伸
                break;
            case Center:
                button_draw_pos.x -= this->size.x / 2;
                draw_pos.x -= text_size.x / 2;
                break;
            case Right:
                button_draw_pos.x -= this->size.x;
                draw_pos.x -= text_size.x;
                break;
            }
            
            // 垂直方向：文本在按钮中垂直居中
            draw_pos.y = button_draw_pos.y + (this->size.y - text_size.y) / 2;
            
            // 边界限制
            button_draw_pos.x = std::max(button_draw_pos.x, 0.0f);
            button_draw_pos.y = std::max(button_draw_pos.y, 0.0f);
            draw_pos.x = std::max(draw_pos.x, 0.0f);
            draw_pos.y = std::max(draw_pos.y, 0.0f);
            
            // 绘制按钮背景（使用偏移后的位置）
            DrawRectangleV(button_draw_pos, this->size, this->tint);
            DrawRectangleLinesEx(Rectangle{button_draw_pos.x, button_draw_pos.y, this->size.x, this->size.y}, 2, DARKGRAY);
            
            // 绘制文本
            DrawTextEx(this->font, this->text.c_str(), draw_pos, this->text_size, this->text_spacing, this->text_tint);
            
            return *this;
        }

        bool button::is_clicked()
        {
            // 计算按钮实际位置（与draw中使用相同的偏移逻辑）
            Vector2 button_pos = this->position;
            
            switch (current_horizontal)
            {
            case Left:
                break;
            case Center:
                button_pos.x -= this->size.x / 2;
                break;
            case Right:
                button_pos.x -= this->size.x;
                break;
            }
            
            Vector2 mouse_pos = GetMousePosition();
            bool hovered = (mouse_pos.x >= button_pos.x && 
                            mouse_pos.x <= button_pos.x + this->size.x &&
                            mouse_pos.y >= button_pos.y && 
                            mouse_pos.y <= button_pos.y + this->size.y);

            if(hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) is_pressed = true;
            return (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        }
    }
    
    namespace font
    {
        Font load_font_chinese(std::string fileName, int fontSize, bool chinese, bool chinese_ex_a, bool chinese_ex_b, bool chinese_symbol)
        {
            // 定义要加载的 Unicode 范围（例如基本汉字 0x4E00-0x9FFF）
            std::vector<int> fontChars;
            for (int i = 32; i < 127; i++) fontChars.push_back(i); // 添加ASCII字符
            for (int i = 0x0021; i <= 0x002F; i++) fontChars.push_back(i); // 常用标点：!"#$%&'()*+,-./
            for (int i = 0x003A; i <= 0x0040; i++) fontChars.push_back(i); // 常用标点：:;<=>?@
            for (int i = 0x005B; i <= 0x0060; i++) fontChars.push_back(i); // 常用标点：[\]^_`
            for (int i = 0x007B; i <= 0x007E; i++) fontChars.push_back(i); // 常用标点：{|}~
            for (int i = 0x2000; i <= 0x206F; i++) fontChars.push_back(i); // 常用标点：通用标点符号（空格、引号、连字符、省略号等）
            for (int i = 0x2070; i <= 0x209F; i++) fontChars.push_back(i); // 上标与下标
            for (int i = 0x00B9; i <= 0x00B9; i++) fontChars.push_back(i); // ¹
            for (int i = 0x00B2; i <= 0x00B3; i++) fontChars.push_back(i); // ²³

            if(chinese) for (int i = 0x4E00; i <= 0x9FFF; i++) fontChars.push_back(i); // 添加汉字
            if(chinese_ex_a) for (int i = 0x3400; i <= 0x4DBF; i++) fontChars.push_back(i); // 扩展A区
            if(chinese_ex_b) for (int i = 0x20000; i <= 0x2A6D6; i++) fontChars.push_back(i); // 扩展B区
            if(chinese_symbol)
            {
                for (int i = 0xFF00; i <= 0xFFEF; i++) fontChars.push_back(i); // 全角标点符号
                for (int i = 0x3000; i <= 0x303F; i++) fontChars.push_back(i); // 中文标点
            }
            fontChars.push_back(0); // 必须包含 0（NULL 终止符）

            // 加载字体
            return LoadFontEx(fileName.c_str(), fontSize, fontChars.data(), fontChars.size());
        }
    }
}