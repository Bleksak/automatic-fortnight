cmake_minimum_required(VERSION 3.1)
project(game C)

set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-O3" "-D _DEBUG")

FILE(COPY ${CMAKE_SOURCE_DIR}/icons DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${CMAKE_SOURCE_DIR}/blocks DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
FILE(COPY ${CMAKE_SOURCE_DIR}/world DESTINATION ${CMAKE_BINARY_DIR})

if (WIN32)
    find_library(GLFW NAMES glfw3.lib)
    find_library(OPENGL NAMES opengl32.lib)
    find_library(GDI NAMES gdi32.lib)
    find_library(CGLM NAMES cglm.lib)
    link_libraries(${GLFW} ${OPENGL} ${GDI})
else (WIN32)
    link_libraries("-lglfw3 -ldl -lX11 -lpthread")
endif (WIN32)

add_executable(game
    src/Blocks.c
    src/Blocks.h
    src/Camera.c
    src/Camera.h
    src/glad.c
    src/GL.c
    src/GL.h
    src/Image.c
    src/Image.h
    src/Input.c
    src/Input.h
    src/main.c
    src/Map.c
    src/Map.h
    src/Shader.c
    src/Shader.h
    src/Textures.c
    src/Textures.h
    src/Window.c
    src/Window.h
)

add_compile_definitions(_CRT_DEPRECATE_TEXT=1 _CRT_SECURE_NO_WARNINGS=1)
set_property(TARGET game PROPERTY C_STANDARD 11)
target_include_directories(game PRIVATE "src")