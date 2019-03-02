#include <AnimationHandler.h>

using namespace Sourcehold::Game;

AnimationHandler::AnimationHandler() {

}

AnimationHandler::AnimationHandler(const AnimationHandler &ani) :
    slots(ani.slots)
{ }

AnimationHandler::~AnimationHandler() {

}

void AnimationHandler::AddSlot(uint16_t slot, const std::pair<uint16_t,uint16_t> range) {
    AnimationSlot::Flags flags = AnimationSlot::NONE;
    slots.insert(
        std::make_pair(
            slot,
            AnimationSlot(SDL_GetTicks(), 0, flags)
        )
    );
}

void AnimationHandler::RemoveSlot(uint16_t slot) {
    slots.erase(slot);
}

uint16_t AnimationHandler::GetFrame(uint16_t slot) {
    std::map<uint16_t, AnimationSlot>::const_iterator it = slots.find(slot);
    if(it != slots.end()) {
        
    }
    return 0;
}

void AnimationHandler::Update() {
    localTime = SDL_GetTicks();
}
