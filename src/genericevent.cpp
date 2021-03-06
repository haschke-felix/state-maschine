#include <genericevent.h>

using namespace event_handling;

GenericEvent::GenericEvent() : triggers_(), triggered_(false), entry_mode_(ForwardEntry) {}

GenericEvent::~GenericEvent() {}

void GenericEvent::clear() {
	for (Trigger* t : triggers_) {
		t->clearTriggered();
	}
	triggered_ = false;
	entry_mode_ = NotOnEntry;
}

bool GenericEvent::process() {
	processTriggers();
	return (process_flags_ & ProcessEveryCycle) || ((process_flags_ & ProcessTriggers) && triggered_);
}

bool GenericEvent::processTriggers() {
	unsigned long time = millis();
	for (Trigger* t : triggers_) {
		if (t->process(time)) {
			triggered_ = true;
		}
	}
	return triggered_;
}

void GenericEvent::addTrigger(Trigger* trigger) {
	triggers_.append(trigger);
}

Trigger* GenericEvent::trigger(const byte index) {
	return triggers_.itemAt((uint16_t)index);
}

void GenericEvent::removeTrigger(const byte index) {
	delete triggers_.itemAt((uint16_t)index);
	triggers_.remove(index);
}

void GenericEvent::removeAllTriggers() {
	for (Trigger* t : triggers_) {
		delete t;
	}
	triggers_.removeAll();
}