

add_executable(thor_demo thor_demo.cc)
target_link_libraries(thor_demo thor)
if (USE_OPENCV)
    target_link_libraries(thor_demo ${OpenCV_LIBS})
endif ()

