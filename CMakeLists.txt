cmake_minimum_required(VERSION 3.26)


# if BUILD_INSTALL = ON -> LauraEngine target defines #define BUILD_INSTALL
# and propagates the macro to Runtime & Editor 
option(BUILD_INSTALL "Use installed paths instead of source-tree paths" ON)


# decide where to ship if --target install
if(NOT DEFINED CMAKE_INSTALL_PREFIX) # if install path not specified
    # use build/install/<config>
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path")
endif()
message(WARNING "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


project(Laura VERSION 1.1.0 LANGUAGES CXX)

# Global C++ settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Laura)         # Engine library (LauraEngine)
add_subdirectory(LauraRuntime)  # Runtime executable (LauraRuntime)
add_subdirectory(LauraEditor)   # Editor executable (LauraEditor)
