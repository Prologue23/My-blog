add_executable(learning_project_3.2.1 opencv_3.2.1.cpp)
add_executable(learning_project_3.2.2 opencv_3.2.2.cpp)

target_link_libraries(learning_project_3.2.1 ${OpenCV_LIBS})
target_link_libraries(learning_project_3.2.2 ${OpenCV_LIBS})

target_include_directories(learning_project_3.2.1 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(learning_project_3.2.2 PRIVATE ${OpenCV_INCLUDE_DIRS})