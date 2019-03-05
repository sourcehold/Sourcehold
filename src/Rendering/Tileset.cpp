#include <Rendering/Tileset.h>

using namespace Sourcehold::Rendering;
using namespace Sourcehold::Parsers;
using namespace Sourcehold::System;

Tileset::Tileset(std::shared_ptr<Renderer> rend) :
    renderer(rend),
    Texture(rend)
{
}

Tileset::~Tileset()  {

}

bool Tileset::Convert(Gm1File &file) {
    if(file.GetType() != Gm1File::Gm1Header::TYPE_TILE) {
        Logger::error("RENDERING") << "Gm1 file given to tileset does not contain tiles!" << std::endl;
        return false;
    }

    /* Allocate image to fit all entries */
    uint32_t nrow = (uint32_t)(std::sqrt((float)file.GetNumEntries()) + 0.5f);
    Texture::AllocNew(30 * nrow, 16 * nrow, SDL_PIXELFORMAT_RGBA8888);

    /* Copy images to tileset */
    uint16_t x = 0, y = 0;
    for(uint32_t i = 0; i < file.GetNumEntries(); i++) {
        Texture &e = file.Get(i);
        e.LockTexture();
        Texture::LockTexture();
        Texture::Copy(e, (x) * 30, (y) * 16);
        e.UnlockTexture();
        Texture::UnlockTexture();

        x++;
        if(x >= nrow) {
            x = 0;
            y++;
        }
    }

    Texture::UpdateTexture();
    return true;
}
