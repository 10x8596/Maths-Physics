# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.16)

# Set the project name
project(3D_Grapher)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/usr/lib/cmake/Qt5")

# Find and include OpenGL, GLFW, and GLEW
find_package(OpenGL REQUIRED)

# Add external libraries
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Add the executable target
add_executable(3d_grapher src/main.cpp)

# Include directiories for GLEW and GLFW
target_include_directories(3d_grapher PRIVATE 
    /usr/include
    /usr/lib/cmake/Qt5
    /usr/lib/qt
)

# Link the libraries
target_link_libraries(3d_grapher
    OpenGL::GL
    glfw
    GLEW::GLEW
    Qt5::Core
    Qt5::Widgets
)
