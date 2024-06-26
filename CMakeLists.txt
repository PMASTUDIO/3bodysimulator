cmake_minimum_required(VERSION 3.28)
project(3bodysimulatornative)

set(CMAKE_CXX_STANDARD 17)

add_executable(3bodysimulatornative main.cpp "vendor/glad/glad.c"
        Application.cpp
        Application.h
        Renderer/Shader.cpp
        Renderer/Shader.h
        Renderer/PrimitiveRenderer.cpp
        Renderer/PrimitiveRenderer.h
        Renderer/Camera.h
        Core/Timestep.h
        Renderer/Camera.cpp
        Simulator/PhysicsBody.cpp
        Simulator/PhysicsBody.h
        Core/Window.h
        Platform/Desktop/DesktopWindow.cpp
        Platform/Desktop/DesktopWindow.h
        Core/Input.h
        Platform/Desktop/DesktopInput.cpp
        Platform/Desktop/DesktopInput.h
        Core/Layer.cpp
        Core/Layer.h
        Core/LayerStack.cpp
        Core/LayerStack.h
        Simulator/SimulationLayer.cpp
        Simulator/SimulationLayer.h)

target_include_directories(3bodysimulatornative PUBLIC vendor/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib)

find_package(OpenGL REQUIRED)

if(APPLE)
    find_package(GLFW3 REQUIRED)
    target_link_libraries(3bodysimulatornative glfw)
elseif(WIN32)
    add_library(glfw STATIC IMPORTED)
    set_target_properties(glfw PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib/glfw3.lib")
    target_link_libraries(3bodysimulatornative glfw)
endif()


target_link_libraries(3bodysimulatornative OpenGL::GL)

add_compile_definitions(GLAD_GL_IMPLEMENTATION GLFW_INCLUDE_NONE GLM_ENABLE_EXPERIMENTAL)
