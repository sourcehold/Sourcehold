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

        /* Ingame dialog windows */
        enum DialogResult {
            IDLE,
            LOAD,
            SAVE,
            BACK,
            QUIT,
            QUIT_MISSION
        };

        DialogResult QuitDialog();
        DialogResult LoadDialog(std::string &name);
        DialogResult CombatMenuDialog();
        DialogResult SiegeMenuDialog();
        DialogResult EconomicsMenuDialog();
        DialogResult SettingsDialog();
        DialogResult EscMenu();

        int GetMilitaryCampaignIndex();
        int GetEconomicsCampaignIndex();
    }
}
