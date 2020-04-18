#pragma once
#include <trigger.h>
#include <list.h>

class GenericEvent
{
public:
	GenericEvent();
	~GenericEvent();
	
	virtual void clear();
	virtual bool process();
	
	// triggers
	void addTrigger(Trigger *trigger);
	Trigger *trigger(const byte index);
	void removeTrigger(const byte index);
	void removeAllTriggers();
	bool triggered() const { return triggered_; }


protected:
	// list of triggers
	List<Trigger *> triggers_;
	// store trigger event
	bool triggered_;

private:
	bool processTriggers();
};