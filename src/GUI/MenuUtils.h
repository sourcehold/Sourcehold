#pragma once

#include <string>
#include <memory>

#include "System/ModCampaign.h"

#include "GUI/Widgets/Table.h"
#include "GUI/Widgets/LineEdit.h"
#include "GUI/Dialog.h"

#include "Events/Event.h"
#include "Events/Mouse.h"

namespace Sourcehold {
    namespace GUI {
        using namespace Events;

        void InitMenuUtils();

        /**
         * Renders the given string to fit the main menu
         * rectangle.
         */
        void RenderMenuText(const std::wstring &text);

        /**
         * Renders the border around the screen. This asset
         * only exists for the HD edition to prevent scaling
         * the interface to fit the screen.
         */
        void RenderMenuBorder();

        /**
         * Given the mouse position relative to the button,
         * check if the mouse collides with the actual
         * image and not the transparent background
         */
        bool CheckButtonCollision(uint32_t rx, uint32_t ry);

        /* Construct common dialog windows */
        std::shared_ptr<Dialog> QuitDialog();
        std::shared_ptr<Dialog> LoadDialog();
        std::shared_ptr<Dialog> CombatMenuDialog();
    }
}
