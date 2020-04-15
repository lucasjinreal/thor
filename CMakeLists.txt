cmake_minimum_required(VERSION 3.5)
project(thor VERSION 1.3.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)


option(USE_OPENCV "enable OpenCV in vis modules" ON)
option(USE_EIGEN "enable build with eigen" OFF)
option(USE_CV_EX "enable additional cv draw ability" OFF)
option(USE_PROTOBUF "enable protobuf related components." OFF)
option(USE_CURL "enable curl related components." ON)
option(DEBUG "Enable debug mode, for develop" ON)


file(GLOB_RECURSE thor_SRCS "src/*.cpp" "src/*.cc")
file(GLOB_RECURSE thor_HEADERS "include/*.h" "include/*.hpp")


if (WIN32)
    message(STATUS "Windows platform thor can only build with simple mode, will close EIGEN and CV_EX")
endif ()

include(GNUInstallDirs)

if (USE_CV_EX)
    include_directories(/usr/include/freetype2/)
    add_definitions(-DUSE_CV_EX)
endif ()

if (USE_EIGEN)
    include_directories(/usr/include/eigen3)
    add_definitions(-DUSE_EIGEN)
endif ()


if (USE_OPENCV)
    find_package(OpenCV REQUIRED)
    if (OpenCV_FOUND)
        message(STATUS "Build with OpenCV")
        message(STATUS "    OpenCV version: " ${OpenCV_VERSION})
        message(STATUS "    OpenCV include path: " ${OpenCV_INCLUDE_DIRS})
        include_directories(${OpenCV_INCLUDE_DIRS})
        if (NOT ${OpenCV_VERSION} LESS "4.0.0")
            add_definitions(-DUSE_OPENCV4)
        endif ()
        add_definitions(-DUSE_OPENCV)
    else ()
        set(USE_OPENCV OFF)
        message(STATUS "Can not found OpenCV, turned it off.")
    endif ()
endif ()


if (USE_CURL)
    find_package(CURL REQUIRED)
    if (NOT CURL_FOUND)
        set(USE_CURL OFF)
        message(STATUS "Can not found curl, turned it off")
    else ()
        add_definitions(-DUSE_CURL)
    endif ()
else()
    message(FATAL_ERROR "thor need build with curl, did u installed libcurl4-dev?")
endif ()


if (USE_PROTOBUF)
    find_package(Protobuf REQUIRED)
    if (NOT PROTOBUF_FOUND)
        set(USE_PROTOBUF OFF)
        message(STATUS "protobuf not found, turned it off.")
    else ()
        add_definitions(-DUSE_PROTOBUF)
    endif ()
endif ()


message(STATUS "OpenCV: " ${USE_OPENCV})
message(STATUS "Eigen: " ${USE_EIGEN})
message(STATUS "Protobuf: " ${USE_PROTOBUF})
message(STATUS "CV_EX: " ${USE_CV_EX})
message(STATUS "CURL: " ${USE_CURL})


add_library(thor SHARED ${thor_SRCS} ${thor_HEADERS} include/image.h src/image.cc include/functions.h src/functions.cc)
add_library(thor_static STATIC ${thor_SRCS} ${thor_HEADERS})

if (USE_OPENCV)
    target_link_libraries(thor ${OpenCV_LIBS})
endif ()

if (USE_PROTOBUF)
    target_link_libraries(thor ${PROTOBUF_LIBRARIES})
endif ()

if (USE_CURL)
    target_link_libraries(thor ${CURL_LIBRARIES})
endif ()

set_target_properties(
        thor
        PROPERTIES
        PUBLIC_HEADER "${thor_HEADERS}")
set_target_properties(
        thor_static
        PROPERTIES
        PUBLIC_HEADER "${thor_HEADERS}")


configure_file(thor.pc thor.pc @ONLY)
target_include_directories(thor PRIVATE .)
target_include_directories(thor_static PRIVATE .)

install(TARGETS thor thor_static
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/thor)
install(FILES include/thor.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
