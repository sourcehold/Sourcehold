#include <AnimationHandler.h>

using namespace Sourcehold::Game;
using namespace Sourcehold::Rendering;

AnimationHandler::AnimationHandler() {

}

AnimationHandler::AnimationHandler(const AnimationHandler &ani) :
    slots(ani.slots)
{ }

AnimationHandler::~AnimationHandler() {

}

void AnimationHandler::AddSlot(uint16_t slot, TextureAtlas &animation, const std::pair<uint16_t,uint16_t> range) {
    slots.insert(std::make_pair(slot, AnimationSlot(0.0, animation.GetSize())));
}

void AnimationHandler::RemoveSlot(uint16_t slot) {
    slots.erase(slot);
}

uint16_t AnimationHandler::GetFrame(uint16_t slot) {
    std::map<uint16_t, AnimationSlot>::const_iterator it = slots.find(slot);
    if(it != slots.end()) {
        return it->second.nframes-1;
    }
    return 0;
}

void AnimationHandler::Update() {

}
