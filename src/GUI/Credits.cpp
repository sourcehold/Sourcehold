#include <GUI/Credits.h>
#include <Rendering/Font.h>
#include <Rendering/Renderer.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Rendering;

/* Not translated, thus hardcoded */
const static wchar_t *credit_lines_firefly[] = {
    L"Game Design",
    L"Simon Bradbury",
    L"",
    L"Additional Design",
    L"Eric Oulette",
    L"",
    L"Game Programming",
    L"Simon Bradbury",
    L"Andrew Prime",
    L"Andrew McNab",
    L"",
    L"Executive Producer",
    L"Eric Oulette",
    L"",
    L"Art Director",
    L"Michael Best",
    L"",
    L"Artists",
    L"Jorge Cameo",
    L"Robert Thornley",
    L"Darren White",
    L"",
    L"Multiplayer Programming",
    L"Andrew Prime",
    L"",
    L"Sound Design and Original",
    L"Music composed by",
    L"Robert L. Euvino",
    L"",
    L"Manual and Story Writer",
    L"Casimir C. Windsor",
    L"",
    L"Quality Assurance Manager",
    L"Darren Thompson",
    L"",
    L"Firefly Testing",
    L"Phil Busuttil",
    L"Casimir C. Windsor",
    L"",
    L"Mandolin",
    L"Matthew Finck",
    L"",
    L"Voice Actors",
    L"Ray Greenoaken",
    L"Jerry Kersey",
    L"James Lawson",
    L"Anthony Mulligan",
    L"John Tearney",
    L"",
    L"Additional Vocals",
    L"Lydia Pidlusky",
    L"",
    L"Scenario Creatorss",
    L"Phil Busuttil",
    L"Eric Oulette",
    L"Darren Thompson",
    L"",
    L"Special Thanks To",
    L"Davis Lester",
};

Credits::Credits() :
    EventConsumer<Mouse>()
{
}

Credits::~Credits()
{
}

bool Credits::Play(bool endgame, bool fadein, bool loop)
{
    playing = true;
    uint8_t currentImage = 0;

    AudioSource music;
    std::shared_ptr<TgxFile> tgx_credits, tgx_1, tgx_2, tgx_3, tgx_4, tgx_firefly;

    if(endgame) {
        music.LoadSong(GetDirectory() / "fx/music/Glory_06.raw", true);
        tgx_credits = GetTgx(GetDirectory() / "gfx/end_credit.tgx");
    }
    else {
        music.LoadSong(GetDirectory() / "fx/music/castlejam.raw", true);
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

        /* Firefly Studios */
        Render(*tgx_firefly, sx, sy);
        sy += 122 + 30;

        for(uint32_t i = 0;
                sy < GetHeight() && i < sizeof(credit_lines_firefly) / sizeof(credit_lines_firefly[0]);
                i++, sy += 46) {
            const wchar_t *line = credit_lines_firefly[i];
            RenderText(line, sx, sy, FONT_SMALL);
        }

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
