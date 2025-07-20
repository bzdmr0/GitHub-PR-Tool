cmake_minimum_required(VERSION 3.16)
project(OpenGL_Test VERSION 1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)
find_package(glfw3 REQUIRED)

# Find OpenGL ES
pkg_check_modules(GLES2 REQUIRED glesv2)
pkg_check_modules(EGL REQUIRED egl)

# Create executable
add_executable(test_app test.c)

# Include directories
target_include_directories(test_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLES2_INCLUDE_DIRS}
    ${EGL_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(test_app 
    glfw
    ${GLES2_LIBRARIES}
    ${EGL_LIBRARIES}
    m  # Math library
)

# Compiler flags
target_compile_options(test_app PRIVATE 
    ${GLES2_CFLAGS_OTHER}
    ${EGL_CFLAGS_OTHER}
    -Wall
    -Wextra
)

# Link directories
target_link_directories(test_app PRIVATE
    ${GLES2_LIBRARY_DIRS}
    ${EGL_LIBRARY_DIRS}
)

# Custom target to run the application
add_custom_target(run
    COMMAND test_app
    DEPENDS test_app
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running OpenGL ES test application"
)

# Installation
install(TARGETS test_app
    RUNTIME DESTINATION bin
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "OpenGL ES 2.0 found: ${GLES2_FOUND}")
message(STATUS "EGL found: ${EGL_FOUND}")
message(STATUS "GLFW3 found: ${glfw3_FOUND}")

# Debug information
message(STATUS "GLES2 include dirs: ${GLES2_INCLUDE_DIRS}")
message(STATUS "GLES2 libraries: ${GLES2_LIBRARIES}")
message(STATUS "EGL libraries: ${EGL_LIBRARIES}")