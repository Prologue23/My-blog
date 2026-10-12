add_executable(opencv_3.1.1 opencv_3.1.1.cpp)
add_executable(opencv_3.1.2 opencv_3.1.2.cpp)

target_link_libraries(opencv_3.1.1 ${OpenCV_LIBS})
target_link_libraries(opencv_3.1.2 ${OpenCV_LIBS})

target_include_directories(opencv_3.1.1 PRIVATE ${OpenCV_INCLUDE_DIRS})
target_include_directories(opencv_3.1.2 PRIVATE ${OpenCV_INCLUDE_DIRS})