cmake_minimum_required(VERSION 3.11)
project(shaderjoy)

# Some compiler stuff
set(SRC_LIST src/main.cpp src/shaderhandler.cpp src/shaderhandler.hpp)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Some CMake settings
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lGL -lGLEW -lSDL2")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Add OpenGL to project
include(FindOpenGL)
find_package(OpenGL REQUIRED)
include(FindGLEW)
find_package(GLEW REQUIRED)
# Add sdl2 to project
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC_LIST})

# Link with all libs
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES})
