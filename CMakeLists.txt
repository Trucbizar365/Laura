cmake_minimum_required(VERSION 3.16)
project(Sandbox VERSION 1.1.0)

add_subdirectory(Laura)

set(EXECUTABLE_NAME "LauraSandbox")
add_executable(${EXECUTABLE_NAME})
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/app/src/*.cpp")
target_sources(${EXECUTABLE_NAME} PRIVATE ${SANDBOX_SOURCES})
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/app/src/")

target_link_libraries(${EXECUTABLE_NAME} PUBLIC LauraEngine)

cmake_minimum_required(VERSION 3.16)
project(App VERSION 1.1.0)
