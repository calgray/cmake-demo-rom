
#include <libdragon.h>
#include <string>

namespace libdragon
{
    class display_context
    {
        display_context_t disp;
        void draw_pixel(int x, int y, uint32_t c );
        void draw_pixel_trans(int x, int y, uint32_t c );
        void draw_line(int x0, int y0, int x1, int y1, uint32_t c );
        void draw_line_trans(int x0, int y0, int x1, int y1, uint32_t c );
        void draw_box(int x, int y, int width, int height, uint32_t color );
        void draw_box_trans(int x, int y, int width, int height, uint32_t color );
        void fill_screen(uint32_t c );
        void set_color( uint32_t forecolor, uint32_t backcolor );
        void draw_character(int x, int y, char c );
        void draw_text(int x, int y, const std::string& msg );
        void draw_sprite(int x, int y, sprite_t& sprite );
        void draw_sprite_stride(int x, int y, sprite_t& sprite, int offset );
        void draw_sprite_trans(int x, int y, sprite_t& sprite );
        void draw_sprite_trans_stride(int x, int y, sprite_t& sprite, int offset );
    };

    class color : public color_t
    {

    };
}