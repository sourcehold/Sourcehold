#include "GUI/NarrScreen.h"
#include "GUI/MenuUtils.h"

#include "Rendering/Font.h"
#include "Rendering/BinkVideo.h"

#include "Parsers/Gm1File.h"
#include "GameManager.h"

using namespace Sourcehold::GUI;
using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

enum class NarrPart {
    NARRATION, // the candle + scrolling text
    STORYSCREEN, // the fireplace / castle + npc dialogue
    NPCINTRO // the introduction to an NPC (snake, wolf, etc.)
};

NarrScreen::NarrScreen(uint8_t mission) :
    mission(mission),
    EventConsumer<Mouse>()
{  
    // TODO
    std::string path = std::string("fx/music/mandloop") +
        (mission % 2 == 0 ? "1.raw" : "2.raw");

    song.Load(GetDirectory() / path, true);

    tgx_bg1 =      GetTgx("gfx/narrbase.tgx");

    /* color */
    tgx_anim[0] =  GetTgx("gfx/flame_01.tgx");
    tgx_anim[1] =  GetTgx("gfx/flame_02.tgx");
    tgx_anim[2] =  GetTgx("gfx/flame_03.tgx");
    tgx_anim[3] =  GetTgx("gfx/flame_04.tgx");
    tgx_anim[4] =  GetTgx("gfx/flame_05.tgx");
    tgx_anim[5] =  GetTgx("gfx/flame_06.tgx");
    tgx_anim[6] =  GetTgx("gfx/flame_07.tgx");
    tgx_anim[7] =  GetTgx("gfx/flame_08.tgx");
    tgx_anim[8] =  GetTgx("gfx/flame_09.tgx");
    tgx_anim[9] =  GetTgx("gfx/flame_10.tgx");
    tgx_anim[10] = GetTgx("gfx/flame_11.tgx");
    tgx_anim[11] = GetTgx("gfx/flame_12.tgx");
    tgx_anim[12] = GetTgx("gfx/flame_13.tgx");
    /* alpha masks */
    tgx_anim[13] = GetTgx("gfx/flame_alpha_01.tgx");
    tgx_anim[14] = GetTgx("gfx/flame_alpha_02.tgx");
    tgx_anim[15] = GetTgx("gfx/flame_alpha_03.tgx");
    tgx_anim[16] = GetTgx("gfx/flame_alpha_04.tgx");
    tgx_anim[17] = GetTgx("gfx/flame_alpha_05.tgx");
    tgx_anim[18] = GetTgx("gfx/flame_alpha_06.tgx");
    tgx_anim[19] = GetTgx("gfx/flame_alpha_07.tgx");
    tgx_anim[20] = GetTgx("gfx/flame_alpha_08.tgx");
    tgx_anim[21] = GetTgx("gfx/flame_alpha_09.tgx");
    tgx_anim[22] = GetTgx("gfx/flame_alpha_10.tgx");
    tgx_anim[23] = GetTgx("gfx/flame_alpha_11.tgx");
    tgx_anim[24] = GetTgx("gfx/flame_alpha_12.tgx");
    tgx_anim[25] = GetTgx("gfx/flame_alpha_13.tgx");
    /* candle */
    tgx_anim2[0] =  GetTgx("gfx/candle_01.tgx");
    tgx_anim2[1] =  GetTgx("gfx/candle_02.tgx");
    tgx_anim2[2] =  GetTgx("gfx/candle_03.tgx");
    tgx_anim2[3] =  GetTgx("gfx/candle_04.tgx");
    tgx_anim2[4] =  GetTgx("gfx/candle_05.tgx");
    tgx_anim2[5] =  GetTgx("gfx/candle_06.tgx");
    tgx_anim2[6] =  GetTgx("gfx/candle_07.tgx");
    tgx_anim2[7] =  GetTgx("gfx/candle_08.tgx");
    tgx_anim2[8] =  GetTgx("gfx/candle_09.tgx");
    tgx_anim2[9] =  GetTgx("gfx/candle_10.tgx");
    tgx_anim2[10] = GetTgx("gfx/candle_11.tgx");
    tgx_anim2[11] = GetTgx("gfx/candle_12.tgx");
    tgx_anim2[12] = GetTgx("gfx/candle_13.tgx");

    skipped = false;
}

NarrScreen::~NarrScreen()
{
}

bool NarrScreen::Begin()
{
    song.Play();
    switch (mission) {
    // TODO: add all campaign missions
    case 1:
        if (!BeginAct(T_START_ACT_ONE)) break;
        if (!BeginNarration()) break;
        if (!BeginStoryScreen(NarrBackground::GOODGUYS)) break;
        break;
    default: break;
    }
    song.Stop();

    return Running();
}

/* TODO: less copy/paste */

void NarrScreen::onEventReceive(Mouse &mouse)
{
    if (mouse.type == MOUSE_BUTTONDOWN) {
        skipped = true;
    }
}

bool NarrScreen::BeginAct(TextSection text)
{
    Uint8 alpha = 255;
    const std::wstring& str = GetString(text, 2);
    auto font = GetGm1("gm/font_stronghold_aa.gm1")->GetTextureAtlas();
    auto dim = GetStringPixelDim(str, FONT_LARGE);

    int px = (GetWidth() / 2) - (dim.first / 2);
    int py = (GetHeight() / 2) - (dim.second / 2);

    double startTime = GetTime();
    while (Running()) {
        if (skipped) {
            skipped = false;
            break;
        }

        double now = GetTime();
        double delta = now - startTime;

        if (delta > 5.0) {
            break;
        }
        if (now < startTime + 1.0) {
            alpha = Uint8(((now - startTime) * 255.0) / 1.0);
        }
        else if (now < startTime + 4.0) {
            alpha = 255;
        }
        else if (now < startTime + 5.0) {
            alpha = 255 - Uint8(((now - (startTime + 4.0)) * 255.0) / 1.0);
        }

        ClearDisplay();

        font->SetAlphaMod(alpha);
        RenderText(str, px, py, FONT_LARGE, false);

        FlushDisplay();
        SyncDisplay();
    }

    font->SetAlphaMod(255);
    return Running();
}

// TODO: fade-in
bool NarrScreen::BeginNarration()
{
    Resolution res = GetResolution();
    StrongholdEdition ed = GetEdition();

    int px = (GetWidth() / 2) - (1024 / 2);
    int py = (GetHeight() / 2) - (768 / 2);

    while (Running()) {
        if (skipped) {
            skipped = false;
            return true;
        }

        ClearDisplay();

        if (ed == STRONGHOLD_HD && res != RESOLUTION_800x600) {
            RenderMenuBorder();
        }

        int index = 1 + (11 - abs(int(GetTime() * 15.0) % (2 * 11) - 11));

        Render(*tgx_bg1, px, py);
        RenderFlameAnim(px, py, index);

        FlushDisplay();
        SyncDisplay();
    }

    return Running();
}

bool NarrScreen::BeginStoryScreen(NarrBackground bg)
{
    BinkVideo bik;
    std::shared_ptr<TgxFile> castle;
    if (bg == NarrBackground::BADGUYS) {
        castle = GetTgx("gfx/storyscreen_castle.tgx");
    }
    else {
        bik.LoadFromDisk(GetDirectory() / "binks/fireplace_01.bik", true);
    }

    int px = (GetWidth()  / 2) - (280 / 2);
    int py = (GetHeight() / 2) - (200 / 2);

    while (Running()) {
        if (skipped) {
            skipped = false;
            return true;
        }

        ClearDisplay();

        if (bg == NarrBackground::BADGUYS) {
            Render(*castle, px, py);
        }
        else {
            bik.Update();
            Render(bik, px, py);
        }

        FlushDisplay();
        SyncDisplay();
    }
    return Running();
}

bool NarrScreen::BeginNpcIntro(NPC npc)
{
    return Running();
}

void NarrScreen::RenderFlameAnim(int px, int py, int index)
{
    auto tgx_color  = tgx_anim [index];
    auto tgx_candle = tgx_anim2[index];
    auto tgx_alpha  = tgx_anim [index + 13];

    // TODO: blending doesn't work

    tgx_alpha->SetBlendMode(SDL_BLENDMODE_ADD);
    Render(*tgx_alpha, px+160, py+235);

    tgx_color->SetBlendMode(SDL_BLENDMODE_ADD);
    Render(*tgx_color, px+160, py+235);

    Render(*tgx_candle, px, py+393);
}
