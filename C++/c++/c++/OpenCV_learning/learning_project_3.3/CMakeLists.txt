file(GLOB_RECURSE SRC_LIST "*.cpp")

add_executable(learning_project_3.3 ${SRC_LIST})

target_link_libraries(learning_project_3.3 ${OpenCV_LIBS})

target_include_directories(learning_project_3.3 PRIVATE 
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)