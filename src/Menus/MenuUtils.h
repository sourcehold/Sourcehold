#pragma once

#include <Rendering/Renderer.h>
#include <Rendering/Font.h>
#include <Parsers/Gm1File.h>
#include <GameManager.h>

namespace Sourcehold
{
    namespace Menus
    {
        using namespace Game;
        bool InitializeUtils(std::shared_ptr<GameManager> mgr) ;
        void RenderMenuText(const std::wstring &text);
    }
}
