#ifndef SCENES_H
#define SCENES_H

#include <glm/glm.hpp>
#include "renderer/Material.h"
#include "Geometry/Sphere.h"

#ifndef SCENEDATA
#define SCENEDATA
struct SceneData {
	const void* sceneObjects;
	size_t size;
	int numberOfObjects;
};
#endif

SceneData defaultScene();
SceneData stanford_dragon_scene();
SceneData stanford_bunny_scene();
SceneData sponza_lights_scene();
SceneData emptyScene();

#endif // SCENES_H