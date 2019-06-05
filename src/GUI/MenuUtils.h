#pragma once

#include <GameManager.h>

#include <Rendering/Renderer.h>
#include <Rendering/Font.h>

#include <GUI/StaticElement.h>

#include <Parsers/Gm1File.h>

namespace Sourcehold
{
    namespace GUI
    {
        using namespace Game;
        bool InitializeUtils(std::shared_ptr<GameManager> mgr) ;
        void RenderMenuText(const std::wstring &text);
    }
}
