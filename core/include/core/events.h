#ifndef EVENTS_H
#define EVENTS_H

#include <GLFW/glfw3.h>
#include <set>
#include <vector>

/**
* @brief EventType is an enum struct that represents the type of an event.
 */
enum struct EventType
{
	KEY_PRESS_EVENT,
	KEY_RELEASE_EVENT,
	KEY_REPEAT_EVENT,
	MOUSE_MOVE_EVENT,
	MOUSE_BUTTON_PRESS_EVENT,
	MOUSE_BUTTON_RELEASE_EVENT,
	MOUSE_SCROLL_EVENT
};

struct Event
{
	EventType type;
};

struct KeyEvent : public Event
{
	int key;
};

struct MouseMoveEvent : public Event
{
	double xpos;
	double ypos;
};

struct MouseButtonEvent : public Event
{
	int button;
};

struct MouseScrollEvent : public Event
{
	double xoffset;
	double yoffset;
};


/**
* @brief EventListener is an interface for classes that want to listen to events.
 * The idea is that the EventDispatcher will call the onEvent method of all the listeners
 * when an event occurs. This way, the listeners can implement their own logic for handling
 * input events.
 */
class EventListener
{
public:
	virtual void onEvent(Event* event) = 0;
};

/**
* @brief EventDispatcher is a class that listens to GLFW events and dispatches them to
 * all the listeners that are registered to it.
 */
class EventDispatcher
{
public:
	EventDispatcher() = default;

	void addListener(EventListener* listener);
	void removeListener(EventListener* listener);
	void notifyListeners(Event* event);

private:
	std::set <EventListener*> m_Listeners;
};

#endif // EVENTS_H