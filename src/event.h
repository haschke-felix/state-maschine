/* event.h : Library provides timers and buttons for statemaschine
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

#pragma once
#include <avr.h>
#include <flags.h>
#include <genericevent.h>

namespace events
{
enum EntryMode
{
	NotOnEntry = 0,
	ForwardEntry = 1,
	BackwardEntry = 2,
};

enum Button
{
	BUTTON_LEFT = _BV(0),
	BUTTON_DOWN = _BV(1),
	BUTTON_UP = _BV(2),
	BUTTON_RIGHT = _BV(3),
	BUTTON_STOP = _BV(4),
	CONTROL_BUTTON = BUTTON_LEFT | BUTTON_DOWN | BUTTON_UP | BUTTON_RIGHT,
	ANY_BUTTON = CONTROL_BUTTON | BUTTON_STOP
};

typedef Flags<Button> Buttons;

enum TriggerFlag
{
	ProcessEveryCycle = _BV(0),
	ProcessPinChanges = _BV(1),
	ProcessTriggers = _BV(2),
	ProcessStop = _BV(3),
	GameMode = (ProcessPinChanges | ProcessTriggers | ProcessStop),
	MenuMode = ProcessPinChanges | ProcessStop,
};

typedef Flags<TriggerFlag> TriggerModes;
} // namespace events

class ButtonAutoTrigger;
class Game;

class Event : public GenericEvent
{
public:
	friend class ButtonAutoTrigger;
	friend class Game;

	Event();
	~Event();

	// button methods
	void processDebounce();
	void checkButtons();

	inline const bool buttonChanged(const events::Button f) const { return button_changes_ & f; }
	inline const bool buttonState(const events::Button f) const { return button_states_.testFlag(f); }
	inline const bool buttonPressed(const events::Button f) const { return button_changes_ & f & button_states_; }
	inline const bool buttonReleased(const events::Button f) const { return button_changes_ & f & ~button_states_; }

	inline const events::EntryMode onEntry() const { return entry_; }
	inline void setOnEntry(const events::EntryMode mode = events::ForwardEntry)
	{
		removeAllTriggers();
		entry_ = mode;
	}

	// trigger structure methods
	bool process() override;
	void clear() override;

	events::Buttons buttonChanges() const { return button_changes_; }

	events::TriggerModes &triggerModes() { return trigger_mode_; }


protected:
	// trigger modes
	events::TriggerModes trigger_mode_;


	// button states
	events::Buttons button_states_;
	events::Buttons button_changes_;
	byte debounce_timers_[5];

	// menu entry mode
	events::EntryMode entry_;
};
