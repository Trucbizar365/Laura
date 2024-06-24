#include "core/events.h"
#include <iostream>


void EventDispatcher::addListener(EventListener* listener)
{
    m_Listeners.insert(listener);
}


void EventDispatcher::removeListener(EventListener* listener)
{
    m_Listeners.erase(listener);
}


void EventDispatcher::notifyListeners(Event* event)
{

    for (EventListener* listener : m_Listeners) 
    {
        listener->onEvent(event);
	}
}




