cmake_minimum_required(VERSION 2.8)

project(RemoteWebcamCli)
project(RemoteWebcamMultiSrv)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

option(DISABLE_GUI_REMOTEWEBCAMMULTISRV "DISABLE_GUI_REMOTEWEBCAMMULTISRV" OFF)
option(DISABLE_TIMER_RECORDING "DISABLE_TIMER_RECORDING" OFF)
option(DISABLE_CUSTOM_BAUDRATE "DISABLE_CUSTOM_BAUDRATE" OFF)
option(USE_OPENCV_HIGHGUI_CPP_API "USE_OPENCV_HIGHGUI_CPP_API" ON)

if(DISABLE_GUI_REMOTEWEBCAMMULTISRV)
	add_definitions(-D DISABLE_GUI_REMOTEWEBCAMMULTISRV)
endif()
if(DISABLE_TIMER_RECORDING)
	add_definitions(-D DISABLE_TIMER_RECORDING)
endif()
if(DISABLE_CUSTOM_BAUDRATE)
	add_definitions(-D DISABLE_CUSTOM_BAUDRATE)
endif()
if(USE_OPENCV_HIGHGUI_CPP_API)
	add_definitions(-D USE_OPENCV_HIGHGUI_CPP_API)
endif()

mark_as_advanced(DISABLE_TIMER_RECORDING DISABLE_CUSTOM_BAUDRATE)

include_directories("${PROJECT_SOURCE_DIR}/../OSUtils")
include_directories("${PROJECT_SOURCE_DIR}/../Extensions/Img")
include_directories("${PROJECT_SOURCE_DIR}/../Extensions/Net")

add_executable(RemoteWebcamCli RemoteWebcamCli/Main.c RemoteWebcamCli/Globals.c ../OSUtils/OSTime.c ../OSUtils/OSNet.c ../OSUtils/OSMisc.c ../OSUtils/OSCore.c)
add_executable(RemoteWebcamMultiSrv RemoteWebcamMultiSrv/Main.cpp RemoteWebcamMultiSrv/Globals.cpp ../OSUtils/OSTimer.c ../OSUtils/OSTime.c ../OSUtils/OSThread.c ../OSUtils/OSNet.c ../OSUtils/OSMisc.c ../OSUtils/OSEv.c ../OSUtils/OSCriticalSection.c ../OSUtils/OSCore.c)

set_source_files_properties(RemoteWebcamCli/Main.c RemoteWebcamCli/Globals.c PROPERTIES LANGUAGE CXX) # Needed for recent versions of OpenCV (> 3.2.0?)...

if(OpenCV_VERSION VERSION_LESS "4")
else()
	if(MSVC)
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # CMAKE_CXX_STANDARD does not seem to work on Mac OS...
	endif()
endif()
target_link_libraries(RemoteWebcamCli ${OpenCV_LIBS})
target_link_libraries(RemoteWebcamMultiSrv ${OpenCV_LIBS})

if(WIN32)
	add_definitions(-D ENABLE_GETTIMEOFDAY_WIN32 -D DISABLE_TIMEZONE_STRUCT_REDEFINITION)
	if(MSVC)
		target_link_libraries(RemoteWebcamCli WinMM WS2_32)
		target_link_libraries(RemoteWebcamMultiSrv WinMM WS2_32)
	else()
		target_link_libraries(RemoteWebcamCli WinMM WS2_32 m)
		target_link_libraries(RemoteWebcamMultiSrv WinMM WS2_32 m)
	endif()
else()
	if(APPLE)
		add_definitions(-D USE_OLD_CHRONO -D USE_OLD_TIMER)
		target_link_libraries(RemoteWebcamCli pthread m)
		target_link_libraries(RemoteWebcamMultiSrv pthread m)
	else()
		target_link_libraries(RemoteWebcamCli pthread rt m)
		target_link_libraries(RemoteWebcamMultiSrv pthread rt m)
	endif()
endif()

set_target_properties(RemoteWebcamCli PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/RemoteWebcamCli")
set_target_properties(RemoteWebcamMultiSrv PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/RemoteWebcamMultiSrv")

# Will only work in Release for MSVC, need to manually remove those lines if needed to build in Debug...
# https://gitlab.kitware.com/cmake/cmake/-/issues/12566
if(WIN32 AND ((CMAKE_BUILD_TYPE MATCHES Release) OR MSVC))
	set_target_properties(RemoteWebcamCli PROPERTIES WIN32_EXECUTABLE TRUE)
	set_target_properties(RemoteWebcamMultiSrv PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
