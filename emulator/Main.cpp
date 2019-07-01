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
    widget_t<display, float> bpm;
    widget_t<display, uint8_t> beats;
    widget_t<display, uint8_t> steps;

    top_bar()
        : bpm
            ( normal_font, normal_fg, normal_bg, 0, 0, lw, h
            , [](auto x) { sprintf(tmp_buf, "%.1f", x); return tmp_buf; }
            , [](auto& x, int i) { x += i; }
            , true
            )
        , beats
            ( normal_font, normal_fg, normal_bg, lw, 0, mw, h
            , [](auto x) { sprintf(tmp_buf, "%d", x); return tmp_buf; }
            , [](auto& x, int i) { x += i; }
            , true
            )
        , steps
            ( normal_font, normal_fg, normal_bg, lw + mw, 0, rw, h
            , [](auto x) { sprintf(tmp_buf, "%d", x); return tmp_buf; }
            , [](auto& x, int i) { x += i; }
            , true
            )
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
        : b0(normal_font, normal_fg, normal_bg, 0, y, lw, h, [](auto _) { return "Run"; }, 0, true)
        , b1(normal_font, normal_fg, normal_bg, lw, y, mw, h, [](auto _) { return "Func"; }, 0, true)
        , b2(normal_font, normal_fg, normal_bg, lw + mw, y, rw, h, [](auto _) { return "Steps"; }, 0, true)
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

enum ui_event_t
    { ui_no_event
    , ui_enc_up
    , ui_enc_dn
    , ui_enc_btn
    , ui_btn_0
    , ui_btn_1
    , ui_btn_2
    , ui_btn_3
    , ui_btn_4
    , ui_btn_5
    , ui_btn_6
    , ui_btn_7
    , ui_btn_a
    , ui_btn_b
    , ui_btn_c
    };

static ui_event_t ui_event()
{
    int x;

    switch (poll_event(x))
    {
    case ev_quit:
        exit(0);
        break;
    case ev_key:
        printf("btn %d\n", x);
        switch (x)
        {
            case '0' : return ui_btn_0;
            case '1' : return ui_btn_1;
            case '2' : return ui_btn_2;
            case '3' : return ui_btn_3;
            case '4' : return ui_btn_4;
            case '5' : return ui_btn_5;
            case '6' : return ui_btn_6;
            case '7' : return ui_btn_7;
            case 'a' : return ui_btn_a;
            case 'b' : return ui_btn_b;
            case 'c' : return ui_btn_c;
            default: ;
        }
        break;
    case ev_wheel:
        printf("wheel = %d\n", x);
        return x > 0 ? ui_enc_up : ui_enc_dn;
        break;
    case ev_btn:
        printf("encoder button!\n");
        return ui_enc_btn;
        break;
    default: ;
    }
}

void run()
{
    display::initialize("Beats 1.0", 1);
    display::clear(black);

    top_bar tb;
    bot_bar bb;

    iwidget *ws[] = { &tb.bpm, &tb.beats, &tb.steps };
    uint8_t nws = sizeof(ws) / sizeof(*ws);

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

    bool edit = false;
    uint8_t focus = 0;

    for (;;)
    {
        ui_event_t ev = ui_event();

        switch (ev)
        {
        case ui_enc_btn:
             edit = !edit;
             if (edit)
                ws[focus]->fgbg(normal_fg, red);
             else
                ws[focus]->fgbg(normal_fg, lime_green);
             break;
        case ui_enc_up: ;
        case ui_enc_dn:
            if (edit)
                ws[focus]->edit(ev == ui_enc_up ? 1 : -1);
            else
            {
                ws[focus]->fgbg(normal_fg, normal_bg);
                if (ev == ui_enc_up)
                    focus = focus + 1 < nws ? focus + 1 : 0;
                else
                    focus = focus > 0 ? focus - 1 : (nws - 1);
                ws[focus]->fgbg(normal_fg, lime_green);
            }
            break;
        default:
            continue;
        }

        display::render();
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

