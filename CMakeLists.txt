cmake_minimum_required(VERSION 3.28)
project(FlappyBird C)

set(CMAKE_C_STANDARD 99)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(external/glfw)

option(CGLM_STATIC "Static build" TRUE)
option(CGLM_USE_C99 "" TRUE)
add_subdirectory(external/cglm)

include_directories(
	include
	external/glfw/include
	external/glad/include
	external/stb/include
	external/cglm/include
)

add_executable (
	${PROJECT_NAME}
	src/main.c
	src/window.c
	src/render.c
	src/shader.c
	src/texture.c
	src/sprite.c
	src/bird.c
	src/pipe.c

	external/glad/src/glad.c
)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL cglm)
