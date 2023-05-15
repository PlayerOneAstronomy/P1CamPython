cmake_minimum_required(VERSION 3.4...3.18)
project(player_one_camera)

add_subdirectory(pybind11)

include_directories("${CMAKE_SOURCE_DIR}/src/include")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "aarch64")
    else()
        set(ARCH "arm")
    endif()
else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH "x86_64")
    else()
        set(ARCH "x86")
    endif()
endif()

if(CMAKE_OSX_ARCHITECTURES)
    if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
        set(ARCH "aarch64")
    elseif(CMAKE_OSX_ARCHITECTURES MATCHES "x86_64")
        set(ARCH "x86_64")
    else()
        message(FATAL_ERROR "Architecture ${CMAKE_OSX_ARCHITECTURES} is not supported.")
    endif()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    if(ARCH STREQUAL "x86")
        set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/lib/linux/x86)
    elseif(ARCH STREQUAL "x86_64")
        set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/lib/linux/x64)
    elseif(ARCH STREQUAL "arm")
        set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/lib/linux/arm32)
    elseif(ARCH STREQUAL "aarch64")
        set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/lib/linux/arm64)
    else()
        message(FATAL_ERROR "Platform is not supported")
    endif()
elseif(WIN32)
    if(ARCH STREQUAL "x86")
        set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/lib/win/x86)
    elseif(ARCH STREQUAL "x86_64")
        set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/lib/win/x64)
    else()
        message(FATAL_ERROR "Platform is not supported")
    endif()
elseif(APPLE)
    set(LIB_PATH ${CMAKE_SOURCE_DIR}/src/lib/mac)
else()
    message(FATAL_ERROR "Platform is not supported")
endif()

find_library(PlayerOneCamera_LIB PlayerOneCamera_Static HINTS ${LIB_PATH})

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # using MT runtime lib on windwos

pybind11_add_module(player_one_camera src/P1Cam.h src/P1Cam.cpp src/CameraPython.cpp)

#set_property(TARGET player_one_camera PROPERTY
#  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(player_one_camera
                            PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
                            
########################################################################
# Find build dependencies
########################################################################
if(NOT WIN32)
	# libusb name for libusb-1.0
    set(libusb1_library_names usb-1.0)
	
	find_package(PkgConfig)

    pkg_check_modules (LIBUSB_PKG libusb-1.0 libusb)

    find_library(LIBUSB_LIBRARIES
    NAMES ${libusb1_library_names}
    PATHS
    ${LIBUSB_PKG_LIBRARY_DIRS}
    /usr/lib
    /usr/local/lib
    )

	if(NOT LIBUSB_LIBRARIES)
		message(FATAL_ERROR "libusb-1.0 is required")
	endif()
endif()
                          

if(NOT WIN32)
    target_link_libraries(player_one_camera PRIVATE ${PlayerOneCamera_LIB} ${LIBUSB_LIBRARIES})
else()
    target_link_libraries(player_one_camera PRIVATE ${PlayerOneCamera_LIB})
endif()
