#pragma once

#include <cinttypes>
#include <memory>

#include <SDL.h>

#include <System/Logger.h>

#include <Rendering/Rendering.h>
#include <Rendering/Camera.h>

namespace Sourcehold
{
    namespace Rendering
    {
        class Texture;
        class Renderer : public Camera
        {
            SDL_Window *window;
            SDL_Renderer *renderer;
            int width, height;
            double tx,ty,tw,th;
            Texture *target = nullptr;
        public:
            Renderer();
            Renderer(const Renderer&) = delete;
            ~Renderer();

            void Init(SDL_Window *window);
            void Update();
            void Clear();
            void Flush();
            void SetTarget(Texture *target, double x, double y, double w, double h);
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

            /**
             * Render a rectangle
             */
            void Render(int x, int y, int w, int h, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255, bool solid = false);
            void Render(double x, double y, double w, double h, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255, bool solid = false);

            void DrawLine(int x1, int y1, int x2, int y2, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
            void DrawLine(double x1, double y1, double x2, double y2, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

            inline int GetWidth() { return width; }
            inline int GetHeight() { return height; }
            inline SDL_Renderer *GetRenderer() { return renderer; }

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

            int GetMouseX();
            int GetMouseY();
        protected:
            static int ResizeEventWatcher(void *data, SDL_Event *event);
        };
    }
}
