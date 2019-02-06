#include <Rendering/StaticElement.h>

using namespace Sourcehold::Rendering;

StaticElement::StaticElement(Context &ctx) : Texture(ctx) {

}

StaticElement::~StaticElement() {

}

void StaticElement::Render() {

}

void StaticElement::Hide() {
    shown = false;
}

void StaticElement::Show() {
    shown = true;
}

void StaticElement::Destroy() {
    
}
