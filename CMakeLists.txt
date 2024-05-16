cmake_minimum_required(VERSION 3.28)
project(3bodysimulatornative)

set(CMAKE_CXX_STANDARD 17)

add_executable(3bodysimulatornative main.cpp "vendor/glad/glad.c"
        Application.cpp
        Application.h
        Renderer/Shader.cpp
        Renderer/Shader.h
        Renderer/PrimitiveRenderer.cpp
        Renderer/PrimitiveRenderer.h)

target_include_directories(3bodysimulatornative PUBLIC vendor/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/lib)

find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

target_link_libraries(3bodysimulatornative glfw)
target_link_libraries(3bodysimulatornative OpenGL::GL)

add_compile_definitions(GLAD_GL_IMPLEMENTATION GLFW_INCLUDE_NONE)
