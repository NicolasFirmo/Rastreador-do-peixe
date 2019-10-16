find_package (Threads)

add_executable( exe Detector.cpp)

target_link_libraries ( exe PUBLIC ${OpenCV_LIBS} )
target_link_libraries ( exe PUBLIC ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries (exe PRIVATE fishtracker)
target_link_libraries (exe PRIVATE vendor)