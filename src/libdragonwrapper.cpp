
#pragma once
#include "libdragonwrapper.h"

namespace libdragon::graphics
{
    color::color(int r, int g, int b, int a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    uint32_t color::color_code() const
    {
        return graphics_convert_color(*this);
    }

    display_context::display_context(display_context_t dc)
    : _disp(dc)
    {}
    display_context::~display_context()
    {
        //display_show(_disp);
    }
    void display_context::set_color( uint32_t forecolor, uint32_t backcolor )
    {
        graphics_set_color(forecolor, backcolor);
    }
    void display_context::draw_pixel(int x, int y, uint32_t c )
    {
        graphics_draw_pixel(_disp, x, y, c);
    }
    void display_context::draw_pixel_trans(int x, int y, uint32_t c )
    {
        graphics_draw_pixel_trans(_disp, x, y, c);
    }
    void display_context::draw_line(int x0, int y0, int x1, int y1, uint32_t c )
    {
        graphics_draw_line(_disp, x0, y0, x1, y1, c);
    }
    void display_context::draw_line_trans(int x0, int y0, int x1, int y1, uint32_t c )
    {
        graphics_draw_line_trans(_disp, x0, y0, x1, y1, c);
    }
    void display_context::draw_box(int x, int y, int width, int height, uint32_t color )
    {
        graphics_draw_box(_disp, x, y, width, height, color);
    }
    void display_context::draw_box_trans(int x, int y, int width, int height, uint32_t color )
    {
        graphics_draw_box_trans(_disp, x, y, width, height, color);
    }
    void display_context::fill_screen(uint32_t c )
    {
        graphics_fill_screen(_disp, c);
    }
    void display_context::draw_character(int x, int y, char c )
    {
        graphics_draw_character(_disp, x, y, c);
    }
    void display_context::draw_text(int x, int y, const std::string& msg )
    {
        graphics_draw_text(_disp, x, y, msg.c_str());
    }
    void display_context::draw_sprite(int x, int y, sprite& sprite )
    {
        graphics_draw_sprite(_disp, x, y, &sprite);
    }
    void display_context::draw_sprite_stride(int x, int y, sprite& sprite, int offset )
    {
        graphics_draw_sprite_stride(_disp, x, y, &sprite, offset);
    }
    void display_context::draw_sprite_trans(int x, int y, sprite& sprite )
    {
        graphics_draw_sprite_trans(_disp, x, y, &sprite);
    }
    void display_context::draw_sprite_trans_stride(int x, int y, sprite& sprite, int offset )
    {
        graphics_draw_sprite_trans_stride(_disp, x, y, &sprite, offset);
    }


    display::display(resolution res, bitdepth bit, uint32_t num_buffers, gamma gamma, antialias aa)
    {
        display_init(
            static_cast<resolution_t>(res),
            static_cast<bitdepth_t>(bit),
            num_buffers,
            static_cast<gamma_t>(gamma),
            static_cast<antialias_t>(aa)
        );
    }
    display::~display()
    {
        display_close();
    }
    display_context display::create_context(bool wait)
    {
        display_context_t dc;
        if (wait)
            while (!(dc = display_lock()));
        else
            dc = display_lock();
        return display_context(dc);
    }
}