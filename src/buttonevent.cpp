#include <buttonevent.h>

using namespace event_handling;

ButtonEvent::ButtonEvent() : GenericEvent(), states_(), changes_() {}

ButtonEvent::~ButtonEvent() {}

void ButtonEvent::clear() {
	GenericEvent::clear();
	changes_.clear();
}

bool ButtonEvent::process() {
	return GenericEvent::process() || ((process_flags_ & ProcessPinChanges) && changes_);
}