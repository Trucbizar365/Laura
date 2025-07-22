#pragma once

#include "Core/Events/IEvent.h"
#include "Scene/Scene.h"

namespace Laura 
{
	struct SceneCreateEvent : public IEvent {
		SceneCreateEvent() = default;
		inline EventType GetType() const override { return EventType::SCENE_CREATE_EVENT; }
	};

	struct SceneCloseEvent : public IEvent {
		SceneCloseEvent() = default;
		inline EventType GetType() const override { return EventType::SCENE_CLOSE_EVENT; }
	};

	struct SceneLoadedEvent : public IEvent {
		std::shared_ptr<Scene> scene;
		SceneLoadedEvent(std::shared_ptr<Scene> scene) : scene(std::move(scene)){}
		inline EventType GetType() const override { return EventType::SCENE_LOADED_EVENT; }
	};
}
