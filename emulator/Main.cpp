#include <widget.h>
#include <emulator.h>
#include <functional>
#include <stdlib.h>

using namespace text;
using namespace color;
using namespace graphics;
using namespace fontlib;

typedef display_t<240, 240> display;

static const color_t normal_bg = web_gray;
static const color_t normal_fg = yellow;
static const font_t& normal_font = fontlib::cmuntt_24;
static char tmp_buf[256];

struct unit_t {};

struct top_bar
{
    widget_t<display, int> bpm;
    widget_t<display, uint8_t> beats;
    widget_t<display, uint8_t> steps;

    top_bar()
        : bpm(normal_font, normal_fg, normal_bg, 0, 0, lw, h, [](auto x) { sprintf(tmp_buf, "%d", x); return tmp_buf; }, true)
        , beats(normal_font, normal_fg, normal_bg, lw, 0, mw, h, [](auto x) { sprintf(tmp_buf, "%d", x); return tmp_buf; }, true)
        , steps(normal_font, normal_fg, normal_bg, lw + mw, 0, rw, h, [](auto x) { sprintf(tmp_buf, "%d", x); return tmp_buf; }, true)
        {}

    static const uint16_t lw = display::width() / 3;
    static const uint16_t rw = lw;
    static const uint16_t mw = display::width() - lw - rw;
    static const uint16_t h = 30;

    static constexpr uint16_t height() { return h; }

    void render()
    {
        bpm.render();
        beats.render();
        steps.render();
    }
};

struct bot_bar
{
    widget_t<display, unit_t> b0;
    widget_t<display, unit_t> b1;
    widget_t<display, unit_t> b2;

    bot_bar()
        : b0(normal_font, normal_fg, normal_bg, 0, y, lw, h, [](auto _) { return "Run"; }, true)
        , b1(normal_font, normal_fg, normal_bg, lw, y, mw, h, [](auto _) { return "Func"; }, true)
        , b2(normal_font, normal_fg, normal_bg, lw + mw, y, rw, h, [](auto _) { return "Steps"; }, true)
        {}

    static const uint16_t lw = display::width() / 3;
    static const uint16_t rw = lw;
    static const uint16_t mw = display::width() - lw - rw;
    static const uint16_t h = 30;
    static const uint16_t y = display::height() - h;

    static constexpr uint16_t height() { return h; }

    void render()
    {
        b0.render();
        b1.render();
        b2.render();
    }
};

template<uint8_t CH>
struct channel_graph
{
    static const uint16_t hpad = 6;
    static const uint16_t w = display::width() / 8;
    static const uint16_t h = display::height() - top_bar::height() - bot_bar::height();
    static const uint16_t x = CH * w + hpad;
    static const uint16_t y = top_bar::height();
    static const uint16_t w_ = w - 2 * hpad;

    static void render_step(uint8_t h_, uint8_t i, bool b)
    {
        if ((i + 1) * h_ >= h)
            return;

        uint8_t ha = h_ >> 1;
        uint8_t hb = h_ - ha;
        uint16_t y_ = y + h - (i + 1) * h_;

        color_t bg = CH == 4 ? red : dark_slate_gray;
        color_t fg = yellow;

        pen_t<display>(bg).fill_rectangle(x, y_, w_, hb);
        pen_t<display>(b ? fg : bg).fill_rectangle(x, y_ + hb, w_, ha);
    }

    static void render(uint64_t s)
    {
        for (uint8_t i = 0; i < 64; ++i)
            render_step(3, i, s & (1ul << i));
    }
};

void run()
{
    display::initialize("Beats 1.0", 1);
    display::clear(black);

    top_bar tb;
    bot_bar bb;

    tb.bpm = 140;

    channel_graph<0>::render(0x1111ul);
    channel_graph<1>::render(0);
    channel_graph<2>::render(0);
    channel_graph<3>::render(0);
    channel_graph<4>::render(0x1111deadbeef1111ul);
    channel_graph<5>::render(0xff);
    channel_graph<6>::render(0);
    channel_graph<7>::render(0);

    display::render();

    bool quit = false;
    const font_t& font = normal_font;
    text_renderer_t<display> tr(font, white, red, true);

    uint16_t r = font.start_row();

    tr.set_pos(0, r);

    while (!quit)
    {
        int x;

        switch (poll_event(x))
        {
        case ev_quit:
            quit = true;
            break;
        case ev_key:
            {
/*
                char c[2] = { static_cast<char>(x), 0 };
                txbox = c;
                display::render();
                */
                break;
            }
        case ev_wheel:
            tb.bpm = tb.bpm + x;
            display::render();
            break;
        case ev_btn:
            tb.beats = tb.beats + 1;
            display::render();
            break;
        default: ;
        }
    }

    display::shutdown();
}

int main()
{
    try
    {
        run();
    }
    catch (const char *s)
    {
        printf("exception: %s\n", s);
    }
}

