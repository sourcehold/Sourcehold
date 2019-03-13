#include <AnimationHandler.h>

using namespace Sourcehold::Game;

AnimationHandler::AnimationHandler() {

}

AnimationHandler::AnimationHandler(const AnimationHandler &ani) :
    slots(ani.slots)
{ }

AnimationHandler::~AnimationHandler() {

}

uint16_t AnimationHandler::AddSlot( const std::pair<uint16_t,uint16_t> range) {
    uint16_t slot = slots.size();

    AnimationSlot::Flags flags = AnimationSlot::NONE;
    slots.insert(
        std::make_pair(
            slot,
            AnimationSlot(
                SDL_GetTicks(),
                range.second > range.first ? range.second - range.first : 1,
            flags)
        )
    );

    return slot;
}

void AnimationHandler::RemoveSlot(uint16_t slot) {
    slots.erase(slot);
}

uint16_t AnimationHandler::GetFrame(uint16_t slot) {
    std::map<uint16_t, AnimationSlot>::const_iterator it = slots.find(slot);
    if(it != slots.end()) {
        return Uint32(localTime * 50.0) % it->second.nframes;
    }
    return 0;
}

void AnimationHandler::Update() {
    localTime = (double)SDL_GetTicks() / 1000.0;
}
