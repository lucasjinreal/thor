cmake_minimum_required(VERSION 3.5)
project(thor VERSION 1.3.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "-fPIC")


option(USE_OPENCV "enable OpenCV in vis modules" ON)
option(USE_EIGEN "enable build with eigen" OFF)
option(USE_CV_EX "enable additional cv draw ability" OFF)
option(USE_PROTOBUF "enable protobuf related components." OFF)
option(USE_CURL "enable curl related components." OFF)
option(BUILD_STATIC "enable to build static thor lib" OFF)
option(DEBUG "Enable debug mode, for develop" ON)


if (WIN32)
    message(STATUS "Windows platform thor can only build with simple mode, will close EIGEN and CV_EX")
endif ()

include(GNUInstallDirs)
include_directories(${PROJECT_SOURCE_DIR}/include/)

if (APPLE)
    # for mac M1 
    include_directories(/opt/homebrew/include/)
endif()

if (USE_CV_EX)
    # include_directories(/usr/include/freetype2/)
    add_subdirectory(3rd/freetype)
    include_directories(3rd/freetype/include)
    add_definitions(-DUSE_CV_EX)
endif ()

if (USE_EIGEN)
    include_directories(/usr/include/eigen3)
    add_definitions(-DUSE_EIGEN)
endif ()

if (USE_OPENCV)
    find_package(OpenCV 4.0 REQUIRED)
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
    message(DEBUG $USE_CURL)
    message(STATUS "built without curl")
endif ()


if (USE_PROTOBUF)
    find_package(Protobuf REQUIRED)
    # NOTE: change to your REAL libprotobuf path
    # set(PROTOBUF_LIBRARIES "/usr/lib/libprotobuf.so")
    message(STATUS " protoc: " ${PROTOBUF_PROTOC_EXECUTE_VERSION} " libprotobuf: " ${Protobuf_VERSION})
    message(STATUS "${Protobuf_LIBRARIES}")
    if (NOT PROTOBUF_FOUND)
        set(USE_PROTOBUF OFF)
        message(STATUS "protobuf not found, turned it off.")
    else ()
        include_directories(${Protobuf_INCLUDE_DIRS})
        set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")
        add_definitions(-DUSE_PROTOBUF)

        set(PROTOBUF_IMPORT_DIRS ${CMAKE_SOURCE_DIR}/include/proto)
        SET(PROTO_META_BASE_DIR ${CMAKE_SOURCE_DIR}/include/proto)
        LIST(APPEND PROTO_FLAGS -I${CMAKE_SOURCE_DIR}/include/proto)

        file(GLOB ProtoFiles "include/proto/*.proto")
        message(STATUS ${ProtoFiles})
        FOREACH(FIL ${ProtoFiles})
            GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)

            string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
            string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})

            string(REGEX MATCH "(/proto/.*|/proto/.*)" OUT_PATH ${FILE_PATH})

            list(APPEND PROTO_SRCS "${CMAKE_SOURCE_DIR}/include/proto/${FIL_WE}.pb.cc")
            list(APPEND PROTO_HDRS "${CMAKE_SOURCE_DIR}/include/proto/${FIL_WE}.pb.h")
            message(STATUS ${PROTO_HDRS})
            EXECUTE_PROCESS(
                    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
            )
        ENDFOREACH()
    endif ()
endif ()

message(STATUS "USE_OPENCV: " ${USE_OPENCV})
message(STATUS "USE_EIGEN: " ${USE_EIGEN})
message(STATUS "USE_PROTOBUF: " ${USE_PROTOBUF})
message(STATUS "USE_CV_EX: " ${USE_CV_EX})
message(STATUS "USE_CURL: " ${USE_CURL})
message(STATUS "BUILD_STATIC: " ${BUILD_STATIC})

file(GLOB_RECURSE thor_SRCS "src/*.cpp" "src/*.cc" "include/proto/*.cc")
file(GLOB_RECURSE thor_HEADERS "include/*.h" "include/*.hpp" "include/thor/proto/*.h")

add_library(thor SHARED ${thor_SRCS} ${thor_HEADERS})
if (BUILD_STATIC)
    add_library(thor_static STATIC ${thor_SRCS} ${thor_HEADERS})
endif()


if (USE_OPENCV)
    target_link_libraries(thor ${OpenCV_LIBS})
endif ()

if (USE_PROTOBUF)
    target_link_libraries(thor ${PROTOBUF_LIBRARIES})
    # target_link_libraries(thor /usr/lib/x86_64-linux-gnu/libprotobuf.so)
    # uncomment this if you are on arm such as JetsonNano or TX2
    # target_link_libraries(thor /usr/lib/aarch64-linux-gnu/libprotobuf.so)
endif ()

if (USE_CURL)
    target_link_libraries(thor ${CURL_LIBRARIES})
endif ()

if (USE_CV_EX)
    target_link_libraries(thor freetype)
endif ()

set_target_properties(
        thor
        PROPERTIES
        PUBLIC_HEADER "${thor_HEADERS}")
target_include_directories(thor PRIVATE .)

if (BUILD_STATIC) 
set_target_properties(
        thor_static
        PROPERTIES
        PUBLIC_HEADER "${thor_HEADERS}")
        target_include_directories(thor_static PRIVATE .)
endif()
configure_file(thor.pc thor.pc @ONLY)

add_subdirectory(examples)

install(TARGETS thor
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/thor)
if (BUILD_STATIC)
install(TARGETS thor_static
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/thor)
endif()
message(STATUS "Proto headers: " ${PROTO_HDRS})
install(FILES ${PROTO_HDRS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/thor/proto)
install(FILES include/thor.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
