#include "core/application_test.h"

#include <iostream>

#include "core/gl_util/OpenGLdebugFuncs.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GUI/CameraComponentGUI.h"
#include "GUI/DockspaceGUI.h"
#include "GUI/PerformanceCounterGUI.h"
#include "GUI/SkyBoxGUI.h"
#include "GUI/BVHsettingsGUI.h"


Application::Application(int screen_width = 1920, int screen_height = 1080): 
	SCREEN_WIDTH(screen_width), 
	SCREEN_HEIGHT(screen_height),


	camera2(90, 16.0 / 9.0, deltaTime2),
	cameraHandler2(camera2),
	sceneData2(emptyScene()),
	active_heuristic2(BVH::Heuristic::SURFACE_AREA_HEURISTIC_BUCKETS),
	//scene_BVH2(BVH::construct(APP_RESOURCES_PATH "models/suzanne_high_poly_rotated.glb", active_heuristic)),
	//scene_BVH2(BVH::construct(APP_RESOURCES_PATH "models/stanford_dragon_pbr.glb", active_heuristic2)),
	//scene_BVH2(BVH::construct(APP_RESOURCES_PATH "models/tiny_house.glb", active_heuristic)),
	//scene_BVH2(BVH::construct(APP_RESOURCES_PATH "models/sponza_scene.glb", active_heuristic)),
	//scene_BVH2(BVH::construct(APP_RESOURCES_PATH "models/stanford_bunny.obj", active_heuristic)),
	scene_BVH2(BVH::construct(APP_RESOURCES_PATH "models/stanford_dragon_pbr.glb", active_heuristic2)),
	shouldPostProcess2(false),
	shouldAccumulate2(true),
	heatmap_color_limit2(350),
	raysPerPixel2(1),
	bouncesPerRay2(5),
	show_skybox2(true),
	reloadSkybox2(false),
	SkyGroundColor2(glm::vec3(0.6392156862f, 0.5803921f, 0.6392156862f)),
	SkyColorHorizon2(glm::vec3(1.0f, 1.0f, 1.0f)),
	SkyColorZenith2(glm::vec3(0.486274509f, 0.71372549f, 234.0f / 255.0f)),
	show_demo_window2(true),
	totalFrames2(0),
	prevCamAspect2(0),
	prevViewportWindowSize2(ImVec2(0.0f, 0.0f)),
	prevViewportWindowPos2(ImVec2(0.0f, 0.0f)),
	display_BVH2(false),
	showPixelData2(true),
	displayed_layer2(1),
	display_multiple2(true),
	BVH_height2(30),
	prev_viewport_mouseX2(0),
	prev_viewport_mouseY2(0),
	inverted_viewport_mouseY2(0),
	AABB_intersect_count_sum2(0),
	TRI_intersect_count_sum2(0),
	same_mouse_pos_count2(0),
	wasGlobalInput2(false), // input from the camera, ImGui or any other source (which should reset accumulation)
	m_NumAccumulatedFrames2(0)
{}

Application::~Application(){}

void Application::run()
{
	init();
	while (!glfwWindowShouldClose(m_Window))
	{
		update();
		render();
	}
	shutdown();
}


void Application::init()
{
	//GLFW WINDOW SETUP
	if (!glfwInit()) 
	{
		glfwTerminate();
		ASSERT(false);
		
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, " ", NULL, NULL);
	if (!m_Window)
	{
		glfwTerminate();
		ASSERT(false);
		
	}

	glfwMakeContextCurrent(m_Window);

	std::cout << glGetString(GL_VERSION) << std::endl;
	glfwSwapInterval(false);

	if (glewInit() != GLEW_OK) 
	{
		if (!m_Window)
		{
			glfwDestroyWindow(m_Window);
			glfwTerminate();
			ASSERT(false);
		}
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	glfwSetWindowUserPointer(m_Window, &cameraHandler2);
	glfwSetKeyCallback(m_Window, CameraHandler::GLFWKeyCallback);
	glfwSetCursorPosCallback(m_Window, CameraHandler::GLFWMousePositionCallback);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 460");


	camera2.posVec = glm::vec3(1.0f, 0.0f, 0.0f);
	renderer2 = new Renderer(sceneData2, scene_BVH2, skyboxFilePath2);
}

void Application::update()
{
	deltaTime2.update();
	totalFrames2 += 1;
	was_ImGui_Input2 = false;

	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Widget windows
	genDockspace();
	component_cameraGUI(camera2, was_ImGui_Input2, cameraHandler2.CameraControllMode, shouldAccumulate2, shouldPostProcess2, raysPerPixel2, bouncesPerRay2);
	genSkyboxGUI(SkyGroundColor2, SkyColorHorizon2, SkyColorZenith2, show_skybox2, was_ImGui_Input2, skyboxFilePath2, reloadSkybox2, cameraHandler2.CameraControllMode);
	BVH_settings_GUI(display_BVH2, active_heuristic2, scene_BVH2.BVH_tree_depth, heatmap_color_limit2, showPixelData2, was_ImGui_Input2, cameraHandler2.CameraControllMode);


	ImGui::ShowDemoWindow(&show_demo_window2);
	if (camera2.cameraKeybinds.camera_keybind_window_active) { genCameraChangeKeybindSplashScreen(); }


	// viewport
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

	ImGui::SetNextWindowDockID(ImGui::GetID("MainDockspace"));

	ImVec2 viewportWindowSize = ImGui::GetContentRegionAvail();

	// check if the viewport size or aspect ratio changed
	if (viewportWindowSize.x != prevViewportWindowSize2.x || viewportWindowSize.y != prevViewportWindowSize2.y || camera2.getAspect() != prevCamAspect2)
	{
		prevViewportWindowSize2 = viewportWindowSize;
		prevCamAspect2 = camera2.getAspect();

		double parentAspect = double(viewportWindowSize.x) / double(viewportWindowSize.y);
		// check if the parent aspect is smaller or equal to camera aspect (width is the limiting factor so the window will span the entire width)
		if (parentAspect <= camera2.getAspect())
		{
			viewportSize2.x = viewportWindowSize.x; // width stays the same
			viewportSize2.y = ceil(viewportWindowSize.x / camera2.getAspect()); // calculate the height based on the aspect ratio
		}
		// (height is the limiting factor so the window will span the entire height)
		else if (parentAspect > camera2.getAspect())
		{
			viewportSize2.y = viewportWindowSize.y; // height stays the same
			viewportSize2.x = ceil(viewportWindowSize.y * camera2.getAspect()); // calculate the width based on the aspect ratio
		}

		renderer2->setViewportSize(glm::vec2(viewportSize2.x, viewportSize2.y)); // sets the texture size to the viewport size

		camera2.setScreenDimentions(int(viewportSize2.x), int(viewportSize2.y));
		was_ImGui_Input2 = true;

		topLeftTextureCoords2.x = (viewportWindowSize.x - viewportSize2.x) / 2.0f;
		topLeftTextureCoords2.y = (viewportWindowSize.y - viewportSize2.y) / 2.0f;
		// viewport offset
		topLeftTextureCoords2.x += ImGui::GetWindowPos().x;
		topLeftTextureCoords2.y += ImGui::GetWindowPos().y;

		bottomLeftTextureCoords2.x = topLeftTextureCoords2.x + viewportSize2.x;
		bottomLeftTextureCoords2.y = topLeftTextureCoords2.y + viewportSize2.y;
	}


	// check if the viewport window position changed
	ImVec2 viewportWindowPos = ImGui::GetWindowPos();
	if (viewportWindowPos.x != prevViewportWindowPos2.x || viewportWindowPos.y != prevViewportWindowPos2.y)
	{
		prevViewportWindowPos2 = viewportWindowPos;

		topLeftTextureCoords2.x = (viewportWindowSize.x - viewportSize2.x) / 2.0f;
		topLeftTextureCoords2.y = (viewportWindowSize.y - viewportSize2.y) / 2.0f;
		// viewport offset
		topLeftTextureCoords2.x += viewportWindowPos.x;
		topLeftTextureCoords2.y += viewportWindowPos.y;

		bottomLeftTextureCoords2.x = topLeftTextureCoords2.x + viewportSize2.x;
		bottomLeftTextureCoords2.y = topLeftTextureCoords2.y + viewportSize2.y;
	}

	camera2.Update();

	wasGlobalInput2 = (camera2.wasCameraInput || was_ImGui_Input2 || !shouldAccumulate2);
	if (wasGlobalInput2) { m_NumAccumulatedFrames2 = 0; }
	else { m_NumAccumulatedFrames2++; }

	viewport_mouseX2 = ImGui::GetMousePos().x - topLeftTextureCoords2.x;
	viewport_mouseY2 = ImGui::GetMousePos().y - topLeftTextureCoords2.y;
	inverted_viewport_mouseY2 = viewportSize2.y - viewport_mouseY2;

	if (reloadSkybox2) {
		renderer2->setSkyboxFilePath(skyboxFilePath2);
		reloadSkybox2 = false;
	}
}

void Application::render()
{
	renderer2->BeginComputeRtxStage();
	renderer2->rtx_uniform_parameters.numAccumulatedFrames = m_NumAccumulatedFrames2;
	renderer2->rtx_uniform_parameters.raysPerPixel = raysPerPixel2;
	renderer2->rtx_uniform_parameters.bouncesPerRay = bouncesPerRay2;
	renderer2->rtx_uniform_parameters.FocalLength = camera2.focalLength;
	renderer2->rtx_uniform_parameters.skyboxGroundColor = SkyGroundColor2;
	renderer2->rtx_uniform_parameters.skyboxHorizonColor = SkyColorHorizon2;
	renderer2->rtx_uniform_parameters.skyboxZenithColor = SkyColorZenith2;
	renderer2->rtx_uniform_parameters.CameraPos = camera2.posVec;
	renderer2->rtx_uniform_parameters.ModelMatrix = glm::mat4(camera2.getModelMatrix());
	renderer2->rtx_uniform_parameters.WasInput = wasGlobalInput2;
			 
	renderer2->rtx_uniform_parameters.display_BVH = display_BVH2;
	renderer2->rtx_uniform_parameters.displayed_layer = displayed_layer2;
	renderer2->rtx_uniform_parameters.display_multiple = display_multiple2;
	renderer2->rtx_uniform_parameters.BVH_tree_depth = scene_BVH2.BVH_tree_depth;
	renderer2->rtx_uniform_parameters.show_skybox = show_skybox2;
	renderer2->rtx_uniform_parameters.heatmap_color_limit = heatmap_color_limit2;
	renderer2->rtx_uniform_parameters.pixelGlobalInvocationID = glm::vec3(viewport_mouseX2, inverted_viewport_mouseY2, 1.0f); // invocations start from bottom left


	// render function
	ComputeTexture* outputTexture = renderer2->RenderComputeRtxStage();

	renderer2->BeginComputePostProcStage();
	renderer2->postProcessing_uniform_parameters.numAccumulatedFrames = m_NumAccumulatedFrames2;
	ComputeTexture* postProcOutput = renderer2->RenderComputePostProcStage();

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddImage((ImTextureID)postProcOutput->ID(), topLeftTextureCoords2, bottomLeftTextureCoords2, { 0, 1 }, { 1, 0 });

	if (ImGui::IsWindowHovered() && showPixelData2 && !cameraHandler2.CameraControllMode) {
		if (viewport_mouseX2 >= 0 && viewport_mouseX2 < viewportSize2.x && inverted_viewport_mouseY2 >= 0 && inverted_viewport_mouseY2 < viewportSize2.y) {
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
			ImGui::BeginTooltip();
			ImGui::BeginGroup();
			ImGui::Spacing();
			ImGui::Text("Pixel Data");
			ImGui::Separator();
			ImGui::Text("X: %d, Y: %d", viewport_mouseX2, inverted_viewport_mouseY2);
			if (wasGlobalInput2 || viewport_mouseX2 != prev_viewport_mouseX2 || viewport_mouseY2 != prev_viewport_mouseY2) {
				AABB_intersect_count_sum2 = 0;
				TRI_intersect_count_sum2 = 0;
				same_mouse_pos_count2 = 0;
			}
			prev_viewport_mouseX2 = viewport_mouseX2;
			prev_viewport_mouseY2 = viewport_mouseY2;

			AABB_intersect_count_sum2 += renderer2->pixelData.AABB_intersect_count;
			TRI_intersect_count_sum2 += renderer2->pixelData.pixelColor.w;
			same_mouse_pos_count2++;


			glm::vec4 pclr = renderer2->pixelData.pixelColor;

			ImVec4 pixelColor = ImVec4(pclr.r, pclr.g, pclr.b, 1.0f);
			ImGui::ColorButton("MyColor##3c", *(ImVec4*)&pclr, NULL, ImVec2(20, 20));
			ImGui::SameLine();
			ImGui::Text("RGB: (%f, %f, %f)", pclr.r, pclr.g, pclr.b);
			ImGui::Text("ray-AABB intersects: %u", AABB_intersect_count_sum2 / same_mouse_pos_count2);
			ImGui::Text("ray-triangle intersects: %u", TRI_intersect_count_sum2 / same_mouse_pos_count2);
			ImGui::EndGroup();
			ImGui::EndTooltip();
			ImGui::PopStyleVar();
		}
	}

	ImGui::PopStyleVar();
	ImGui::End();

	genPerformanceCounter();
	camera2.ResetFlags();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Application::shutdown()
{
	delete renderer2;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

Application* createApplication()
{
	return new Application();
}
