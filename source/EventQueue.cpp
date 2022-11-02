#include "..\include\EventQueue.h"

std::vector<Event> EventQueue::m_events;

EventQueue::EventQueue()
{

}

EventQueue::~EventQueue()
{

}

void EventQueue::f_addEvent(Event e)
{
	EventQueue::m_events.push_back(e);
}

Event EventQueue::f_fetchEvent()
{

	if (m_events.size() > 0)
	{
		Event result = m_events[0];
		EventQueue::m_events.erase(m_events.begin());
		return result;
	}		
	else
	{
		return Event::NONE;
	}
		
}