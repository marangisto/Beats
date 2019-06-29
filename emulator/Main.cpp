#include <text.h>
#include <fontlib.h>
#include <draw.h>
#include <emulator.h>

using namespace text;
using namespace color;
using namespace graphics;
using namespace fontlib;

typedef display_t<240, 240> display;

static const color_t normal_bg = web_gray;
static const font_t& normal_font = fontlib::cmuntt_24;

void text_box
    ( uint16_t      x
    , uint16_t      y
    , uint16_t      w
    , uint16_t      h
    , const font_t& font
    , color_t       fg
    , color_t       bg
    , const char    *s
    )
{
    text_renderer_t<display> tr(font, fg, bg, true);
    pen_t<display> pen(bg);
    uint16_t tw, th;
 
    tr.bounding_box(s, tw, th);

    uint16_t rpad = tw < w ? (w - tw) >> 1 : 0;
    uint16_t lpad = tw < w ? w - tw - rpad : 0;

    if (lpad)
        pen.fill_rectangle(x, y, lpad, h);
    if (rpad)
        pen.fill_rectangle(x + lpad + tw, y, rpad, h);

    uint16_t bpad = th < h ? (h - th) >> 1 : 0;
    uint16_t tpad = th < h ? h - th - bpad : 0;

    if (tpad)
        pen.fill_rectangle(x + lpad, y, tw, tpad);
    if (bpad)
        pen.fill_rectangle(x + lpad, y + th + tpad, tw, bpad);

    pen_t<display>(black).rectangle(x, y, w, h);
    tr.set_pos(x + lpad, y + tpad - font.min_y);
    tr.write(s);
}

struct top_bar
{
    static const uint16_t lw = display::width() / 3;
    static const uint16_t rw = display::width() - lw;
    static const uint16_t h = 30;

    static constexpr uint16_t height() { return h; }

    static void render()
    {
        text_box(0,  0, lw, h, normal_font, yellow, normal_bg, "140.0");
        text_box(lw, 0, rw, h, normal_font, yellow, normal_bg, "1: (5, 14)");
    }
};

struct bot_bar
{
    static const uint16_t lw = display::width() / 3;
    static const uint16_t rw = lw;
    static const uint16_t mw = display::width() - lw - rw;
    static const uint16_t h = 30;
    static const uint16_t y = display::height() - h;

    static constexpr uint16_t height() { return h; }

    static void render()
    {
        text_box(0,  y, lw, h, normal_font, yellow, normal_bg, "Run");
        text_box(lw, y, mw, h, normal_font, yellow, normal_bg, "Func");
        text_box(lw + mw, y, rw, h, normal_font, yellow, normal_bg, "Steps");
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
        //pen_t<display>(orange_red).fill_rectangle(x, y, w_, h);

        for (uint8_t i = 0; i < 64; ++i)
            render_step(3, i, s & (1ul << i));
    }
};

void run()
{
    display::initialize("Beats 1.0", 1);
    display::clear(black);

    top_bar::render();
    bot_bar::render();
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
        char c[2] = { 0, 0 };

        if (keyboard_poll(c[0]))
            switch (c[0])
            {
            case 0:
                quit = true;
                break;
            case '\r':
                r += font.line_spacing();
                tr.set_pos(0, r);
                break;
            default:
                text_box(160, 50, 50, 40, font, dim_gray, wheat, c);
                display::render();
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

