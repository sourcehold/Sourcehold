#pragma once

#include <string>
#include <cinttypes>
#include <SDL.h>

#include "Rendering/Texture.h"
#include "Rendering/Shapes.h"

namespace Sourcehold {
    namespace Rendering {
        enum Resolution : uint8_t {
            RESOLUTION_800x600 = 0,
            RESOLUTION_1024x768 = 1,
            RESOLUTION_1280x720 = 2,
            RESOLUTION_1280x1024 = 3,
            RESOLUTION_1360x768 = 4,
            RESOLUTION_1366x768 = 5,
            RESOLUTION_1440x900 = 6,
            RESOLUTION_1600x900 = 7,
            RESOLUTION_1600x1200 = 8,
            RESOLUTION_1680x1050 = 9,
            RESOLUTION_1920x1080 = 10,
        };

        bool InitRenderer();
        void DestroyRenderer();
        void UpdateRenderer();
        void ClearDisplay();
        void FlushDisplay();

        /**
         * Redirect all rendering operations to the target
         */
        void SetTarget(Texture *target, Rect<double> rect);
        void SetTarget(Texture *target, Rect<int> rect);
        void ResetTarget();

        /**
         * Render a texture (screen coordinates)
         */
        void Render(Texture &texture, int x, int y, SDL_Rect *clip = nullptr);
        void Render(Texture &texture, int x, int y, int w, int h, SDL_Rect *clip = nullptr);

        /**
         * Render a texture (normalized coordinates)
         */
        void Render(Texture &texture, double x, double y, SDL_Rect *clip = nullptr);
        void Render(Texture &texture, double x, double y, double w, double h, SDL_Rect *clip = nullptr);

        /**
         * Render a texture to the whole screen
         */
        void Render(Texture &texture, SDL_Rect *clip = nullptr);
        void Fill(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);

        void RenderRect(Rect<int> rect, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255, bool solid = false);
        void RenderRect(Rect<double> rect, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255, bool solid = false);

        void RenderLine(Line<int> line, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
        void RenderLine(Line<double> line, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

        /**
         * Get custom blend mode for keying out
         * certain pixels based on src
         */
        SDL_BlendMode GetAlphaKeyBlendMode();

        /**
         * Normalize based on the dimensions of
         * the current render target
         */
        double NormalizeX(int32_t c);
        double NormalizeY(int32_t c);
        int32_t ToCoordX(double c);
        int32_t ToCoordY(double c);
        double NormalizeTargetX(int32_t c);
        double NormalizeTargetY(int32_t c);
        int32_t ToTargetCoordX(double c);
        int32_t ToTargetCoordY(double c);

        double GetTargetWidth();
        double GetTargetHeight();
        double GetTargetX();
        double GetTargetY();
        SDL_Renderer *GetRenderer();
    }
}
