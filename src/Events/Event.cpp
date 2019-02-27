#include <Events/Event.h>

using namespace Sourcehold::Events;

Event::Event() {
    
}

Event::Event(const std::string &name) {
    this->name = name;
}

Event::Event(const Event &event) {

}

Event::~Event() {

}

