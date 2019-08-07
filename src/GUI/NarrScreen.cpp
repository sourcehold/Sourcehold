#include <GUI/NarrScreen.h>
#include <GUI/MenuUtils.h>

#include <GameManager.h>

using namespace Sourcehold::GUI;
using namespace Sourcehold::Game;

NarrScreen::NarrScreen(TextSection sec) :
    EventConsumer<Mouse>()
{
    tgx_bg = GetTgx(GetDirectory() / "gfx/narrbase.tgx");

    /* color */
    tgx_anim[0] =  GetTgx(GetDirectory() / "gfx/flame_01.tgx");
    tgx_anim[1] =  GetTgx(GetDirectory() / "gfx/flame_02.tgx");
    tgx_anim[2] =  GetTgx(GetDirectory() / "gfx/flame_03.tgx");
    tgx_anim[3] =  GetTgx(GetDirectory() / "gfx/flame_04.tgx");
    tgx_anim[4] =  GetTgx(GetDirectory() / "gfx/flame_05.tgx");
    tgx_anim[5] =  GetTgx(GetDirectory() / "gfx/flame_06.tgx");
    tgx_anim[6] =  GetTgx(GetDirectory() / "gfx/flame_07.tgx");
    tgx_anim[7] =  GetTgx(GetDirectory() / "gfx/flame_08.tgx");
    tgx_anim[8] =  GetTgx(GetDirectory() / "gfx/flame_09.tgx");
    tgx_anim[9] =  GetTgx(GetDirectory() / "gfx/flame_10.tgx");
    tgx_anim[10] = GetTgx(GetDirectory() / "gfx/flame_11.tgx");
    tgx_anim[11] = GetTgx(GetDirectory() / "gfx/flame_12.tgx");
    tgx_anim[12] = GetTgx(GetDirectory() / "gfx/flame_13.tgx");
    /* alpha masks */
    tgx_anim[13] = GetTgx(GetDirectory() / "gfx/flame_alpha_01.tgx");
    tgx_anim[14] = GetTgx(GetDirectory() / "gfx/flame_alpha_02.tgx");
    tgx_anim[15] = GetTgx(GetDirectory() / "gfx/flame_alpha_03.tgx");
    tgx_anim[16] = GetTgx(GetDirectory() / "gfx/flame_alpha_04.tgx");
    tgx_anim[17] = GetTgx(GetDirectory() / "gfx/flame_alpha_05.tgx");
    tgx_anim[18] = GetTgx(GetDirectory() / "gfx/flame_alpha_06.tgx");
    tgx_anim[19] = GetTgx(GetDirectory() / "gfx/flame_alpha_07.tgx");
    tgx_anim[20] = GetTgx(GetDirectory() / "gfx/flame_alpha_08.tgx");
    tgx_anim[21] = GetTgx(GetDirectory() / "gfx/flame_alpha_09.tgx");
    tgx_anim[22] = GetTgx(GetDirectory() / "gfx/flame_alpha_10.tgx");
    tgx_anim[23] = GetTgx(GetDirectory() / "gfx/flame_alpha_11.tgx");
    tgx_anim[24] = GetTgx(GetDirectory() / "gfx/flame_alpha_12.tgx");
    tgx_anim[25] = GetTgx(GetDirectory() / "gfx/flame_alpha_13.tgx");
    /* candle */
    tgx_anim2[0] =  GetTgx(GetDirectory() / "gfx/candle_01.tgx");
    tgx_anim2[1] =  GetTgx(GetDirectory() / "gfx/candle_02.tgx");
    tgx_anim2[2] =  GetTgx(GetDirectory() / "gfx/candle_03.tgx");
    tgx_anim2[3] =  GetTgx(GetDirectory() / "gfx/candle_04.tgx");
    tgx_anim2[4] =  GetTgx(GetDirectory() / "gfx/candle_05.tgx");
    tgx_anim2[5] =  GetTgx(GetDirectory() / "gfx/candle_06.tgx");
    tgx_anim2[6] =  GetTgx(GetDirectory() / "gfx/candle_07.tgx");
    tgx_anim2[7] =  GetTgx(GetDirectory() / "gfx/candle_08.tgx");
    tgx_anim2[8] =  GetTgx(GetDirectory() / "gfx/candle_09.tgx");
    tgx_anim2[9] =  GetTgx(GetDirectory() / "gfx/candle_10.tgx");
    tgx_anim2[10] = GetTgx(GetDirectory() / "gfx/candle_11.tgx");
    tgx_anim2[11] = GetTgx(GetDirectory() / "gfx/candle_12.tgx");
    tgx_anim2[12] = GetTgx(GetDirectory() / "gfx/candle_13.tgx");
}

NarrScreen::~NarrScreen()
{
}

bool NarrScreen::Begin()
{
    Resolution res = GetResolution();
    StrongholdEdition ed = GetEdition();

    int px = (GetWidth() / 2) - (1024 / 2);
    int py = (GetHeight() / 2) - (768 / 2);

    while(Running()) {
        ClearDisplay();

        if(ed == STRONGHOLD_HD && res != RESOLUTION_800x600) {
            RenderMenuBorder();
        }

        int index = 1 + (11 - abs(int(GetTime() * 15.0) % (2 * 11) - 11));

        Render(*tgx_bg, px, py);
        RenderFlameAnim(px, py, index);

        FlushDisplay();
        SyncDisplay();
    }

    return Running();
}

void NarrScreen::onEventReceive(Mouse &mouse)
{
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
