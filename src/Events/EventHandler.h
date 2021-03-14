#pragma once

#include <functional>

namespace Sourcehold {
namespace Events {
/**
 * Dispatch new events. Returns false
 * if quit event was received
 */
bool FetchEvents();

int AddEventListener(std::function<void(SDL_Event&)> fn);
void RemoveEventListener(int id);
}  // namespace Events
}  // namespace Sourcehold