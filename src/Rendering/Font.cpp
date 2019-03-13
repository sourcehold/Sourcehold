#include <Rendering/Font.h>

using namespace Sourcehold::Rendering;

Font::Font(std::shared_ptr<Renderer> rend) {
    this->renderer = rend;
}

Font::~Font() {

}
