#include <StaticElement.h>

using namespace Sourcehold::Game;

StaticElement::StaticElement() {

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