#include <Texture.h>

using namespace OpenSH::Rendering;

Texture::Texture(Context &ctx) {

}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}
