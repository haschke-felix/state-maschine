#pragma once
#include <flags/intflags.h>
#include <genericevent.h>

class ButtonAutoTrigger;
class ButtonEvent : public GenericEvent
{
public:
	friend class ButtonAutoTrigger;
	ButtonEvent();
	~ButtonEvent();

	virtual inline const bool buttonChanged(const uint32_t& button) const { return changes_ & button; }
	virtual inline const bool buttonState(const uint32_t& button) const { return states_ & button; }
	virtual inline const bool buttonPressed(const uint32_t& button) const { return changes_ & states_ & button; }
	virtual inline const bool buttonReleased(const uint32_t& button) const { return changes_ & (~states_) & button; }

	virtual void clear() override;
	virtual bool process() override;

protected:
	IntFlags<uint32_t> states_;
	IntFlags<uint32_t> changes_;
};