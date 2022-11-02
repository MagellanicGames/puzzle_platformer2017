#pragma once
#include <vector>

enum Event
{
	NONE,STARTGAME, QUITGAME, STARTLEVEL,LEVELSTARTSCREEN,CHANGELEVEL, NEWLEVELADDED, RESTARTLEVEL,LEVELCOMPLETED,
	OPENOPTIONS,CHANGERES,CHANGEFULLSCREEN,CHANGEWINDOWED,MAINMENU,SHOWCOLLISIONTILES, HIDECOLLISIONTILES,
	MOVELEFT,MOVERIGHT,MOVEDOWN,MOVEUP,RETURNOVERWORLD,CHANGEPAR,PLAYIMPACTSOUND
};

class EventQueue
{
public:

	EventQueue();
	~EventQueue();

	static void f_addEvent(Event e);
	static Event f_fetchEvent();

	static int f_numEvents() { return EventQueue::m_events.size(); }

private:

	static std::vector<Event> m_events;
};