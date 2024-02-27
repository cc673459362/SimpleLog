cmake_minimum_required (VERSION 3.6)
project("example")

if (WIN32)
    message("build win platform..")
    include_directories(${PROJECT_SOURCE_DIR})

    # add files into project
    file(GLOB LOG_FILE "${PROJECT_SOURCE_DIR}/log/*.h"
                            "${PROJECT_SOURCE_DIR}/log/*.cc")
    source_group(log FILES ${LOG_FILE})
    file(GLOB TIME_FILE "${PROJECT_SOURCE_DIR}/time/*.h"
                            "${PROJECT_SOURCE_DIR}/time/*.cc")
    source_group(time FILES ${TIME_FILE})

    message("build win example")
    add_definitions(-DUNICODE -D_UNICODE)

    set(exe_name win_example)
    add_executable(${exe_name} example.cc ${LOG_FILE} ${TIME_FILE})
elseif (APPLE)
        message("build mac platform")
        set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")
        include_directories(${PROJECT_SOURCE_DIR})

        # add files into project
        file(GLOB LOG_FILE "${PROJECT_SOURCE_DIR}/log/*.h"
                                "${PROJECT_SOURCE_DIR}/log/*.cc")
        source_group(log FILES ${LOG_FILE})
        file(GLOB TIME_FILE "${PROJECT_SOURCE_DIR}/time/*.h"
                                "${PROJECT_SOURCE_DIR}/time/*.cc")
        source_group(time FILES ${TIME_FILE})

        message("build win example")
        add_definitions(-DUNICODE -D_UNICODE)

        set(exe_name mac_example)
        add_executable(${exe_name} example.cc ${LOG_FILE} ${TIME_FILE})
else()
    message("unsupport platform")
endif()
