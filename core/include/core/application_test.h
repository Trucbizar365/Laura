#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "deltaLib/DeltaTime.h"
#include "core/camera/CameraHandler.hpp" // also includes Camera.hpp
#include "scenes/Scene1.hpp"
#include "core/Renderer.h"

class Application
{

public:
	const int SCREEN_WIDTH, SCREEN_HEIGHT;
private:
	GLFWwindow* m_Window;
    DeltaTime deltaTime2;
    Camera camera2;
    CameraHandler cameraHandler2;
    SceneData sceneData2;
    BVH::Heuristic active_heuristic2;
    BVH::BVH_data scene_BVH2;
    std::string skyboxFilePath2;
    Renderer* renderer2;

    bool was_ImGui_Input2;
    bool shouldPostProcess2;
    bool shouldAccumulate2;
    int heatmap_color_limit2;
    int raysPerPixel2;
    int bouncesPerRay2;
    bool show_skybox2;
    bool reloadSkybox2;
    glm::vec3 SkyGroundColor2;
    glm::vec3 SkyColorHorizon2;
    glm::vec3 SkyColorZenith2;
    bool show_demo_window2;
    int totalFrames2;
    double prevCamAspect2;
    ImVec2 prevViewportWindowSize2;
    ImVec2 prevViewportWindowPos2;
    ImVec2 viewportSize2;
    ImVec2 topLeftTextureCoords2;
    ImVec2 bottomLeftTextureCoords2;
    bool display_BVH2;
    bool showPixelData2;
    int displayed_layer2;
    bool display_multiple2;
    int BVH_height2;
    unsigned int viewport_mouseX2;
    unsigned int viewport_mouseY2;
    unsigned int prev_viewport_mouseX2;
    unsigned int prev_viewport_mouseY2;
    unsigned int inverted_viewport_mouseY2;
    unsigned int AABB_intersect_count_sum2;
    unsigned int TRI_intersect_count_sum2;
    unsigned int same_mouse_pos_count2;
    bool wasGlobalInput2; // input from the camera, ImGui or any other source (which should reset accumulation)
    unsigned int m_NumAccumulatedFrames2;

public:
	Application(int screen_width, int screen_height);
	virtual ~Application();
	void run();

protected:
	virtual void init();
	virtual void update();
	virtual void render();
	virtual void shutdown();
};

Application* createApplication();

#endif // APPLICATION_H