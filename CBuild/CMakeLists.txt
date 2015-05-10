cmake_minimum_required(VERSION 2.8.3)
project(RebopAPI)

set(CXXFLAGS ${CXXFLAGS} -fPIC)
# Linux specific flags
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	set(CXXFLAGS ${CXXFLAGS} -fPIC)
	add_definitions(-std=c++0x)
endif()

find_package(
  Threads
)

set(REBOP_DIR "..")
set(rebop_INCLUDE_DIRS 
  ${REBOP_DIR}/Source
  ${REBOP_DIR}/ThirdPartyLibs/ArDroneSDK3/include
  ${REBOP_DIR}/ThirdPartyLibs/easyloggingpp/include
  ${REBOP_DIR}/ThirdPartyLibs/rapidjson/include
)

include_directories(
  ${rebop_INCLUDE_DIRS}
)

add_library(bebop_interface
  ${REBOP_DIR}/Source/CBebopInterface.cpp
  ${REBOP_DIR}/Source/CVehicleInterface.cpp
  ${REBOP_DIR}/Source/CNetworkInterface.cpp
  ${REBOP_DIR}/Source/CNetworkSettings.cpp
  ${REBOP_DIR}/Source/CCommandPacket.cpp
  ${REBOP_DIR}/Source/Utility.cpp
)

add_library(ardrone_sdk STATIC IMPORTED)

set_property(TARGET ardrone_sdk PROPERTY
  IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/${REBOP_DIR}/ThirdPartyLibs/ArDroneSDK3/lib/libarsdk3.a)

target_link_libraries(bebop_interface
  ardrone_sdk
  ${CMAKE_THREAD_LIBS_INIT}
)
