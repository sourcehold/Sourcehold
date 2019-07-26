#pragma once

#include <string>

namespace Sourcehold {
    namespace GUI {
        /**
         * Init menu-related stuff which will remain loaded
         * for the entire duration of the game
         */
        bool InitializeUtils();

        /**
         * Renders the given string to fit the main menu
         * rectangle.
         * TODO: replace with complete text box function
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

        /**
         * Renders every style of ingame menu box, in which
         * text and / or the menu can be placed.
         */
        enum MenuBox {
            MENUBOX_LOAD_GAME, /* Load a map or savegame */
            MENUBOX_BUILDER, /* The title box for builder menu pages */
            MENUBOX_CAMPAIGN, /* Combat or economics campaign selection */
            MENUBOX_ENDGAME, /* 'Victory' or 'Defeat' screen */
            MENUBOX_ESC /* The ingame Esc-menu */
        };
        void RenderMenuBox(MenuBox style, const std::wstring &title);
    }
}
