cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 14)

project(npsipsdk)

set (CMAKE_INCLUDE_CURRENT_DIR ON)

set(XML_SRC_DIR "${PROJECT_SOURCE_DIR}/tinyxml2")
include_directories(${XML_SRC_DIR})

set(SDK_SRC_DIR ./source)
set(MESSAGE_SRC_DIR ./Message)
include_directories(./header )



aux_source_directory(${XML_SRC_DIR} XML_SRCS)
aux_source_directory(${SDK_SRC_DIR} SDK_SRCS)
aux_source_directory(${MESSAGE_SRC_DIR} MESSAGE_SRCS)
#aux_source_directory(. SRCS)
set(SOURCES ${XML_SRCS} ${SDK_SRCS} ${MESSAGE_SRCS})

add_definitions(-DOUT)
link_libraries(libresip.so  libresipares.so librutil.so libdum.so)
add_library(npsipsdk STATIC ${SOURCES})

#include_directories(./hksdk/include)
#include_directories(./ffmpeg-3.4.2-win64-dev/include)
#include_directories(.)
#aux_source_directory(. SRCS )

#link_directories(  ${CMAKE_CURRENT_BINARY_DIR}/libs ${CMAKE_CURRENT_BINARY_DIR}/hksdk/libs ${CMAKE_CURRENT_BINARY_DIR}/ffmpeg-3.4.2-win64-dev/lib)
#link_directories(/opt/CH_HCNetSDK_V5.2.7.4_build20170606_Linux64/lib /opt/CH_HCNetSDK_V5.2.7.4_build20170606_Linux64/lib/HCNetSDKCom)
#add_definitions(-DHKDEVICE_EXPORTS -DGLOG_NO_ABBREVIATED_SEVERITIES -DGOOGLE_GLOG_DLL_DECL= -D__x86_64__)

#link_libraries(pthread glog  avformat avcodec avutil swresample z )
#link_libraries(/opt/CH_HCNetSDK_V5.2.7.4_build20170606_Linux64/lib/libhcnetsdk.so)
#add_executable(${PROJECT_NAME} ${SRCS} )
#target_link_libraries( ${PROJECT_NAME} libhcnetsdk.so)