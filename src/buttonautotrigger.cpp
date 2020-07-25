/* buttonautotrigger.cpp : for automove with two buttons
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

#include <buttonautotrigger.h>

using namespace event_handling;

ButtonAutoTrigger::ButtonAutoTrigger(ButtonEvent* event, const uint32_t button1, const uint32_t button2,
                                     const unsigned int first_interval, const unsigned int interval)
  : event_(event)
  , button1_(button1)
  , button2_(button2)
  , activated_(true)
  , dir_(NO_BTN)
  , first_interval_(first_interval)
  , interval_(interval) {}
bool ButtonAutoTrigger::process(const unsigned long& time) {
	if (!activated_) {
		timer_.clearTriggered();
		return false;
	}

	// buttons
	if (event_->buttonChanged(button1_)) {
		if (event_->buttonState(button1_)) {
			if (!event_->buttonState(button2_)) {
				event_->changes_.setFlag(button1_);
				triggered_ = true;
				onTriggered();
				timer_.setInterval(first_interval_);
				timer_.start();
				dir_ = BTN_1;
			}
		} else if (dir_ == BTN_1) {
			timer_.clearTriggered();
			if (event_->buttonState(button2_)) {
				dir_ = BTN_2;
				timer_.setInterval(first_interval_);
				timer_.start();
			} else {
				dir_ = NO_BTN;
				timer_.stop();
			}
		}
	}

	else if (event_->buttonChanged(button2_)) {
		if (event_->buttonState(button2_)) {
			if (!event_->buttonState(button1_)) {
				event_->changes_.setFlag(button2_);
				triggered_ = true;
				onTriggered();
				timer_.setInterval(first_interval_);
				timer_.start();
				dir_ = BTN_2;
			}
		} else if (dir_ == BTN_2) {
			timer_.clearTriggered();
			if (event_->buttonState(button1_)) {
				dir_ = BTN_1;
				timer_.setInterval(first_interval_);
				timer_.start();
			} else {
				timer_.stop();
				dir_ = NO_BTN;
			};
		}
	}

	// timer
	else if (timer_.process(time)) {
		timer_.clearTriggered();
		event_->changes_.setFlag((dir_ == BTN_1 ? button1_ : button2_));
		triggered_ = true;
		onTriggered();
		timer_.setInterval(interval_);
		timer_.restart();
	}
	return triggered_;
}

void ButtonAutoTrigger::stop() {
	activated_ = false;
	timer_.stop();
}

void ButtonAutoTrigger::start() {
	activated_ = true;
}
