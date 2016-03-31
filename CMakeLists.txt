cmake_minimum_required(VERSION 2.4)

### link opentoonz_plugin_utility
add_subdirectory(opentoonz_plugin_utility)
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(PLATFORM1 32)
        set(PLATFORM2)
    else()
        set(PLATFORM1 64)
        set(PLATFORM2 _64)
    endif()

    if(PLATFORM1 EQUAL 64)
        add_definitions(-Dx64)
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /openmp")
    set(PLUGIN_UTILITY_LIB "${CMAKE_CURRENT_SOURCE_DIR}/opentoonz_plugin_utility/lib/${CMAKE_CFG_INTDIR}/libopentoonz_plugin_utility")
endif(WIN32)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")
    set(PLUGIN_UTILITY_LIB "${CMAKE_CURRENT_SOURCE_DIR}/opentoonz_plugin_utility/lib/${CMAKE_CFG_INTDIR}/libopentoonz_plugin_utility.a")
endif(APPLE)

find_package(OpenCV REQUIRED)
set(LIBS ${OpenCV_LIBS} ${PLUGIN_UTILITY_LIB})
include_directories(opentoonz_plugin_utility/include
        opentoonz_plugin_utility/opentoonz_plugin_headers
        "${OpenCV_INCLUDE_DIRS}")
link_directories("${OpenCV_LIBS}")

### Compile plugin
set(PLUGIN_NAME kmeans_posterize)
set(PLUGIN_VENDOR hiking)
set(SOURCES
        src/main.cpp)

add_library(${PLUGIN_NAME} SHARED ${HEADERS} ${SOURCES} )

set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX "${PLUGIN_VENDOR}_"
        SUFFIX "_${CMAKE_SYSTEM_NAME}.plugin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_definitions(-DPLUGIN_NAME="${PLUGIN_NAME}")
add_definitions(-DPLUGIN_VENDOR="${PLUGIN_VENDOR}")

target_link_libraries(${PLUGIN_NAME} ${LIBS})