/* buttonautotrigger.h : for automove with two buttons
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
#include <buttonevent.h>
#include <timer.h>
#include <trigger.h>

class ButtonAutoTrigger : public Trigger
{
public:
	ButtonAutoTrigger(ButtonEvent* event, const uint32_t button1, const uint32_t button2,
	                  const unsigned int first_interval = 200, const unsigned int interval = 150);
	// trigger public interface

	void stop() override;
	void start() override;

	enum Direction
	{
		BTN_1,
		BTN_2,
		NO_BTN,
	};

	Direction direction() { return dir_; }

	void setInterval(const unsigned int interval) { interval_ = interval; }
	void setFirstInterval(const unsigned int interval) { first_interval_ = interval; }

	// Trigger interface
protected:
	bool process(const unsigned long& time) override;

private:
	Timer timer_;

	ButtonEvent* event_;

	const uint32_t button1_;
	const uint32_t button2_;

	bool activated_ = true;
	Direction dir_;

	unsigned int first_interval_;
	unsigned int interval_;
};
