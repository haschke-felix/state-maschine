/* event.cpp : Library provides timers and buttons for statemaschine
 *
 * Copyright (C) 2019 Felix Haschke
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see http://www.gnu.org/licenses/.
 */

#include <event.h>

using namespace events;

Event::Event() :  trigger_mode_(), button_states_(), button_changes_(), debounce_timers_{0,0,0,0,0}, entry_(NotOnEntry) {
	DDRC = 0b0000;
	PORTC = 0b1111;
	DDRB = 0b10;
	PORTB = 0b10;
}

Event::~Event(){

}

bool readButton(Button b) {
	return !(b == BUTTON_STOP ? bitRead(PINB, 1) : b & PINC);
}

void Event::processDebounce() {
	for (int i = 0; i <= 4; i++) {
		if (debounce_timers_[i] > 0) {
			debounce_timers_[i]--;
			if (debounce_timers_[i] == 0) {
				Button button = Button(_BV(i));
				bool read = readButton(button);
				if (read != button_states_.testFlag(button)) {
					button_states_.setFlag(button, read);
				}
				button_changes_.setFlag(button);
			}
		}
	}
}

void Event::checkButtons() {
	for (int i = 0; i <= 4; i++) {
		Button b = Button(_BV(i));
		if (bool(debounce_timers_[i]) != (readButton(b) != button_states_.testFlag(b)))
			debounce_timers_[i] = 20;
	}
}

void Event::clear() {
	GenericEvent::clear();
	button_changes_.clear();
	entry_ = NotOnEntry;
}

bool Event::process() {
	GenericEvent::process();
	return ((trigger_mode_ & ProcessEveryCycle) ||
	        ((trigger_mode_ & ProcessPinChanges) && (button_changes_ & CONTROL_BUTTON)) ||
	        ((trigger_mode_ & ProcessTriggers) && triggered_) ||
	        ((trigger_mode_ & ProcessStop) && buttonChanged(BUTTON_STOP)));
}