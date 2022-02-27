
#include <libdragon.h>
#include <string>

namespace libdragon::graphics
{
    enum class resolution
    {
        /** @brief 320x240 mode */
        res_320x240 = RESOLUTION_320x240,
        /** @brief 640x480 mode */
        res_640x480 = RESOLUTION_640x480,
        /** @brief 256x240 mode */
        res_256x240 = RESOLUTION_256x240,
        /** @brief 512x480 mode */
        res_512x480 = RESOLUTION_512x480,
        /** @brief 512x240 mode, high-res progressive */
        res_512x240 = RESOLUTION_512x240,
        /** @brief 640x240 mode, high-res progressive */
        res_640x240 = RESOLUTION_640x240,
    };

    enum class bitdepth
    {
        depth_16_bpp = DEPTH_16_BPP,
        depth_32_BPP = DEPTH_32_BPP
    };

    enum class gamma
    {
        none = GAMMA_NONE,
        correct = GAMMA_CORRECT,
        correct_dither = GAMMA_CORRECT_DITHER
    };

    enum class antialias
    {
        off = ANTIALIAS_OFF,
        resample = ANTIALIAS_RESAMPLE,
        resample_fetch_needed = ANTIALIAS_RESAMPLE_FETCH_NEEDED,
        resameple_fetch_always = ANTIALIAS_RESAMPLE_FETCH_ALWAYS
    };

    class color : private color_t
    {
    public:
        color(int r, int g, int b, int a);
        uint32_t color_code() const;
    };

    class sprite : protected sprite_t
    {

    };

    class display_context
    {
        display_context_t _disp;
    public:
        display_context(int dc);
        ~display_context();
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
        void draw_sprite(int x, int y, sprite& sprite );
        void draw_sprite_stride(int x, int y, sprite& sprite, int offset );
        void draw_sprite_trans(int x, int y, sprite& sprite );
        void draw_sprite_trans_stride(int x, int y, sprite& sprite, int offset );
    };

    class display
    {
        display(resolution res, bitdepth bit, uint32_t num_buffers, gamma gamma, antialias aa);
        ~display();
        display_context create_context(bool wait);
    };
}



namespace libdragon::interrupt
{
    // MIPS interface (MI)
    // timer interrupt (TI)

    // video interface (VI)
    // audio interface (AI)
    // serial interface (SI) (controller, rumble, ctrl pak)
    // parallel interface (PI) (game pak, DD)
}

namespace libdragon::controller
{
    class controller_n64 : protected SI_condat
    {

    };

    class controller_gc : protected _SI_condat_gc
    {

    };

    enum class dpad_direction : int
    {
        right = 0,
        up_right = 1,
        up = 2,
        up_left = 3,
        left = 4,
        down_left = 5,
        down = 6,
        down_right = 7
    };

    enum class controller_status
    {
        none = 0,
        controller_1_inserted = CONTROLLER_1_INSERTED,
        controller_2_inserted = CONTROLLER_2_INSERTED,
        controller_3_inserted = CONTROLLER_3_INSERTED,
        controller_4_inserted = CONTROLLER_4_INSERTED,
    };

    enum class accessory
    {
        none = ACCESSORY_NONE,
        mempak = ACCESSORY_MEMPAK,
        rumblepak = ACCESSORY_RUMBLEPAK,
        vru = ACCESSORY_VRU,
        transferpak = ACCESSORY_TRANSFERPAK
    };

    class controllers : protected SI_controllers_state_t
    {
    public:
        controllers();
        void read();
        void read_gc();
        void read_gc_origin();
        void scan();

        static controller_status get_controllers_present();
        static controller_status get_accesories_present(controller_data* out);

        controller_data get_keys_pressed() const;
        controller_data get_keys_down() const;
        controller_data get_keys_up() const;
        controller_data get_keys_held() const;
        
        dpad_direction get_dpad_direction(int controller) const;

        int read_mempak_address(int controller, uint16_t address, uint8_t* data);
        int write_mempak_address(int controller, uint16_t address, uint8_t *data);

        static accessory identify_accessory(int controller) const;

        void rumble_start(int controller);
        void rumble_stop(int controller);

        static void execute_raw_command(
            int controller, int command,
            int bytesout, int bytesin,
            unsigned char* out, unsigned char* in);
    };

    class controller_origins : protected SI_controllers_origin_t
    {

    };
}

namespace libdragon::audio
{
    class audio
    {
    public:
        audio(const int frequency, int numbuffers);
        ~audio();

        void set_buffer_callback(audio_fill_buffer_callback fill_buffer_callback);
        void pause(bool pause);
        void write(const short* const buffer);
        volatile int can_write();
        void write_silence();
        int get_frequency();
        int get_buffer_length();

        short* write_begin();
        void write_end();
    }
}

namespace libdragon::rdp
{

}
namespace libdragon::rsp
{

}

//architecture
// CPU, RCP, RDRAM, Device

//RPC (Reality Co-Processor)
// has 2 processors, RSP and RDP
// RSP - Reality Signal Processor - Audio, fixed-point geometry and culling
// RDP - Reality Display Processor - Graphics

// RDRAM - main memory shared by CPU and RCP

//DMA - transfers between device and memory
// without using CPU



// threads
// priority ordered execution

// initializing thread
// game thread
// device thread
// scheduler thread
// graphics thread
// audio thread

//osCreateThread()
//osStartThread()
//osStopThread()