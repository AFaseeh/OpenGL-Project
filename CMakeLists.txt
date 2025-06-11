# Source files
set(SOURCES
Source.cpp
glad.c
Shader.cpp
stb_image_init.cpp
)

# Create the executable
add_executable(MyApp ${SOURCES})

add_subdirectory(dependencies/includes/glm)

target_include_directories(MyApp PRIVATE
    ${CMAKE_SOURCE_DIR}/dependencies/includes/
	${CMAKE_SOURCE_DIR}/dependencies/includes/glad
    ${CMAKE_SOURCE_DIR}/dependencies/includes/GLFW
    ${CMAKE_SOURCE_DIR}/dependencies/includes/KHR
	${CMAKE_SOURCE_DIR}/dependencies/lib/
)

target_link_libraries(MyApp
	glm
    ${CMAKE_SOURCE_DIR}/dependencies/lib/glfw3.lib
    opengl32
    user32
    gdi32
    shell32
)