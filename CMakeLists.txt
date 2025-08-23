cmake_minimum_required(VERSION 3.26)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
project(LauraEditor VERSION 1.1.0)

# LAURA RUNTIME TARGET ///////////////////
add_executable(LauraRuntime)
set_property(TARGET LauraRuntime PROPERTY CXX_STANDARD 20)
file(GLOB_RECURSE RUNTIME_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/LauraRuntime/src/*.cpp")
target_sources(LauraRuntime PRIVATE ${RUNTIME_SOURCES})
target_include_directories(LauraRuntime PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/LauraRuntime/src/")
target_link_libraries(LauraRuntime PUBLIC LauraEngine)

set_target_properties(LauraRuntime PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/Laura/src/res/ExportRuntime"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/Laura/src/res/ExportRuntime"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/Laura/src/res/ExportRuntime"
)

if(MSVC)
	target_compile_definitions(LauraRuntime PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_property(TARGET LauraRuntime PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	if (NOT ENABLE_CONSOLE)
		set_target_properties(LauraRuntime PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
endif()


# LAURA EDITOR TARGET ///////////////////
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/Thirdparty/imgui-docking") # UI
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/Thirdparty/implot") # Graphs and plotting
add_subdirectory(Laura)
option(ENABLE_CONSOLE "Enable console" ON)
add_executable(LauraEditor)
set_property(TARGET LauraEditor PROPERTY CXX_STANDARD 20)
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/src/*.cpp")
# in case visual studio again adds some files to the EDITOR_SOURCES dont remove the entire line, just the path to the file !!!
target_sources(LauraEditor PRIVATE ${EDITOR_SOURCES})
target_include_directories(LauraEditor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/src/")
target_link_libraries(LauraEditor PUBLIC imgui implot LauraEngine)
add_dependencies(LauraEditor LauraRuntime) # to ensure runtime gets built on `cmake --build .`

target_compile_definitions(LauraEditor PUBLIC
    $<$<CONFIG:Debug>:EDITOR_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/res/">
    $<$<CONFIG:RelWithDebInfo>:EDITOR_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/res/">
    $<$<CONFIG:Release>:EDITOR_RESOURCES_PATH="./editor_res/">
)

# Copy Editor resources next to the main executable
add_custom_command(TARGET LauraEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LauraEditor>/editor_res"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/LauraEditor/res"
        "$<TARGET_FILE_DIR:LauraEditor>/editor_res"
    COMMENT "Copying editor resources next to the executable"
)

# Copy engine resources next to the main executable
add_custom_command(TARGET LauraEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:LauraEditor>/engine_res"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Laura/src/res"
        "$<TARGET_FILE_DIR:LauraEditor>/engine_res"
    COMMENT "Copying engine resources next to the executable"
)

if(MSVC)
	target_compile_definitions(LauraEditor PUBLIC _CRT_SECURE_NO_WARNINGS)
	set_property(TARGET LauraEditor PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	if (NOT ENABLE_CONSOLE)
		set_target_properties(LauraEditor PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
endif()