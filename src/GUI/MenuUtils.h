#pragma once

#include <string>

#include "System/ModCampaign.h"

#include "GUI/Table.h"
#include "GUI/LineEdit.h"

#include "Events/Event.h"
#include "Events/Mouse.h"

namespace Sourcehold {
    namespace GUI {
        using namespace Events;
        using namespace System;

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

        /* Ingame dialog windows */
        enum DialogResult {
            IDLE,
            LOAD,
            SAVE,
            BACK,
            QUIT,
            QUIT_MISSION
        };

        enum class DialogType {
            // Original game //
            QUIT,
            LOAD,
            COMBAT_MENU,
            SIEGE_MENU,
            ECO_MENU,
            SETTINGS,
            ESC_MENU,
            // Sourcehold //
            CAMPAIGN_SELECT_MILITARY,
            CAMPAIGN_SELECT_ECO
        };

        class DialogWindow : protected EventConsumer<Mouse> {
            bool clicked=false;
            Table table;
            LineEdit lineEdit;
            DialogType type;

            enum class State {
                GAME_OPTIONS,
                GAMEPLAY_OPTIONS,
                VIDEO_OPTIONS,
                SOUND_OPTIONS,
                CHANGE_NAME,
                SAVE,
                LOAD,
                OPTIONS,
                HELP,
                RESTART_MISSION,
                QUIT_MISSION,
                EXIT_STRONGHOLD
            } state = State::GAME_OPTIONS;
        protected:
            void onEventReceive(Mouse &event) override;

            // TODO: names?
            enum DialogButton : uint8_t {
                BUTTON_1 = 0,
                BUTTON_2,
                BUTTON_3,
                BUTTON_4,
                BUTTON_5,
                BUTTON_6,
                BUTTON_7,
                BUTTON_8,
                BUTTON_9
            };

            enum class Deco {
                LARGE,
                SMALL
            };

            bool RenderButton(DialogButton style, const std::wstring& text, uint32_t x, uint32_t y);
            void RenderDialogBorder(int x, int y, int nx, int ny);
            void RenderDeco(Deco type, int x, int y);
            void RenderDialogTextBox(int x, int y, int w, int h, const std::wstring &text, bool deco);

            DialogResult QuitDialog();
            DialogResult LoadDialog();
            DialogResult CombatMenuDialog();
            DialogResult SiegeMenuDialog();
            DialogResult EconomicsMenuDialog();
            DialogResult SettingsDialog();
            DialogResult EscMenu();
            DialogResult QuitMissionDialog();

            void InitModCampaignTable(std::vector<ModCampaign> *campaigns);
        public:
            DialogWindow() = default;
            DialogWindow(DialogType type);
            ~DialogWindow() = default;

            void Init(DialogType type);
            void Reset();

            // Render the dialog window in its current state //
            DialogResult Render();

            // Get index of the selected table entry //
            int GetSelectedIndex();

            inline void Open() { open = true; }
            inline void Close() { open = false; }

            bool open;
        };
    }
}
