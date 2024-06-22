#ifndef SCENES_H
#define SCENES_H

#include <glm/glm.hpp>


#ifndef RTX_MATERIAL
#define RTX_MATERIAL
struct RaytracingMaterial
{
    // Thanks to https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL

    glm::vec3 color;              // offset 0   // alignment 16 // size 12 // total 12 bytes
    float emissionStrength;       // offset 12  // alignment 4  // size 4  // total 16 bytes
    glm::vec3 emissionColor;      // offset 16  // alignment 16 // size 12 // total 28 bytes
    float std140padding;          // offset 28  // alignment 4  // size 4  // total 32 bytes
};
#endif

struct Sphere {
	RaytracingMaterial material;  // offset 0   // alignment 16 // size 32 // total 32 bytes
	glm::vec3 position;           // offset 32  // alignment 16 // size 12 // total 44 bytes
	float radius;                 // offset 44  // alignment 4  // size 4  // total 48 bytes
};

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