cmake_minimum_required(VERSION 3.10)
project(OpenCV_learning)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED OpenCV_DIR)
    message(FATAL_ERROR "OpenCV_DIR is not set. Pass the directory containing OpenCVConfig.cmake.")
endif()

find_package(OpenCV 4 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_subdirectory(learning_project_3.1)
add_subdirectory(learning_project_3.2)
add_subdirectory(learning_project_3.3)
