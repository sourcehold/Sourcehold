#include <GUI/Credits.h>

#include <Rendering/Font.h>
#include <Rendering/Renderer.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

Credits::Credits() :
    EventConsumer<Mouse>()
//    layout(GetHlpSection(L"credits.hlp"), { 0.5, 0.0, 0.5, 1.0 })
{
}

Credits::~Credits()
{
}

bool Credits::Play(bool endgame, bool fadein, bool loop)
{
    playing = true;
    uint8_t currentImage = 0;

    Song music;
    std::shared_ptr<TgxFile> tgx_credits, tgx_1, tgx_2, tgx_3, tgx_4, tgx_firefly;

    if(endgame) {
        music.Load(GetDirectory() / "fx/music/Glory_06.raw", true);
        tgx_credits = GetTgx(GetDirectory() / "gfx/end_credit.tgx");
    }
    else {
        music.Load(GetDirectory() / "fx/music/castlejam.raw", true);
        tgx_1 = GetTgx(GetDirectory() / "gfx/credits_1.tgx");
        tgx_2 = GetTgx(GetDirectory() / "gfx/credits_2.tgx");
        tgx_3 = GetTgx(GetDirectory() / "gfx/credits_3.tgx");
        tgx_4 = GetTgx(GetDirectory() / "gfx/credits_4.tgx");
    }

    tgx_firefly = GetTgx(GetDirectory() / "gfx/front_firefly_logo.tgx");

    music.Play();

    Uint8 alpha = 255;
    double startTime = GetTime();
    double fadeBase = startTime;

    int iw = endgame ? 1024 : 800, ih = endgame ? 768 : 600;
    int px = (GetWidth() / 2) - (iw / 2);
    int py = (GetHeight() / 2) - (ih / 2);

    int sx = (GetWidth() / 2) + (200 / 2);
    int scrollOffset = 0;

    MouseOff();

    Resolution res = GetResolution();
    while (Running() && playing) {
        ClearDisplay();

        double now = GetTime();

        if(now < fadeBase + 2.0) {
            alpha = Uint8(((now - fadeBase) * 255.0) / 2.0);
        }
        else if(now < fadeBase + 16.0) {
            alpha = 255;
        }
        else if(now < fadeBase + 18.0) {
            alpha = 255 - Uint8(((now - (fadeBase + 16.0)) * 255.0) / 2.0);
        }
        else if(now > fadeBase + 18.0) {
            alpha = 0;
            currentImage = (currentImage + 1) % 4;
            fadeBase = now;
        }

        if(endgame) {
            if(res == RESOLUTION_800x600) {
                /* Scale down */
                Render(*tgx_credits);
            }
            else {
                /* Place in the middle */
                Render(*tgx_credits, px, py);
            }
        }
        else {
            switch(currentImage) {
            case 0: {
                tgx_1->SetAlphaMod(alpha);
                Render(*tgx_1, px, py);
            }
            break;
            case 1: {
                tgx_2->SetAlphaMod(alpha);
                Render(*tgx_2, px, py);
            }
            break;
            case 2: {
                tgx_3->SetAlphaMod(alpha);
                Render(*tgx_3, px, py);
            }
            break;
            case 3: {
                tgx_4->SetAlphaMod(alpha);
                Render(*tgx_4, px, py);
            }
            break;
            default:
                break;
            }
        }

        /* Render the scroller */
        int sy = GetHeight() - scrollOffset;
        scrollOffset = int((now - startTime) * 60.0);

        layout.Render(sx, sy);

        // TODO: Rest of credits, colored titles

        FlushDisplay();
        SyncDisplay();
    }

    MouseOn();
    music.Stop();

    return true;
}

void Credits::onEventReceive(Mouse &event)
{
    if(event.LmbDown() || event.RmbDown()) {
        playing = false;
    }
}
