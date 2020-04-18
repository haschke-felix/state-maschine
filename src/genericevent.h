#pragma once
#include <flags/intflags.h>
#include <list.h>
#include <trigger.h>

namespace event_handling {
enum EntryMode
{
	NotOnEntry,
	ForwardEntry,
	BackwardEntry,
	UserEntry,
};
enum TriggerFlag
{
	ProcessEveryCycle = _BV(0),
	ProcessPinChanges = _BV(1),
	ProcessTriggers = _BV(2),
};
};  // namespace event_handling

class GenericEvent
{
public:
	GenericEvent();
	~GenericEvent();

	virtual void clear();
	virtual bool process();

	// triggers
	void addTrigger(Trigger* trigger);
	Trigger* trigger(const byte index);
	void removeTrigger(const byte index);
	void removeAllTriggers();
	bool triggered() const { return triggered_; }

	inline const uint8_t onEntry() const { return entry_mode_; }
	inline void setEntry(uint8_t entry_mode = event_handling::ForwardEntry) {
		removeAllTriggers();
		entry_mode_ = entry_mode;
	}

	void clearProcessFlags() { process_flags_.clear(); }
	inline void setProcessFlag(uint8_t flag) { process_flags_.setFlag(flag); }

protected:
	// list of triggers
	List<Trigger*> triggers_;
	// store trigger event
	bool triggered_;
	uint8_t entry_mode_;
	IntFlags<uint8_t> process_flags_;

private:
	bool processTriggers();
};