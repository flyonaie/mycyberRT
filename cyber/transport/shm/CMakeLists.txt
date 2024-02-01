cmake_minimum_required(VERSION 3.5.1)

project(cybershm VERSION 8.0.0) # Apollo Version

set(CMAKE_CXX_STANDARD 14)
set(TARGET_NAME ${PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug) # Debug/Release
endif()

enable_language(ASM)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CROUTINE_FILE "cyber/croutine/detail/swap_x86_64.S")
if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
  set(CROUTINE_FILE "cyber/croutine/detail/swap_x86_64.S")
  message("### SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR} ${CROUTINE_FILE}")
else()
  set(CROUTINE_FILE "cyber/croutine/detail/swap_aarch64.S")
  message("### SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR} ${CROUTINE_FILE}")
endif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")

find_package(Threads REQUIRED)

message("### include_directories: ${CMAKE_CURRENT_SOURCE_DIR}")
message("### include_directories: ${PROJECT_SOURCE_DIR}")


# include_directories(/usr/local/workspace/github/CyberRT-9.0.0/cyber/transport/shm)
# include_directories(${PROJECT_SOURCE_DIR})
# link_directories(${CMAKE_CURRENT_SOURCE_DIR})


file(GLOB SHM_COM_SRCS
    "cyber/transport/shm/*.cc"
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../../
)

# list(FILTER SHM_COM_SRCS EXCLUDE REGEX .*test[.]cc)

# add_library(${TARGET_NAME} SHARED
#     ${CYBER_SRCS}
# )

add_executable(${TARGET_NAME} shm_test.cc ${SHM_COM_SRCS})
target_link_libraries(${TARGET_NAME}
    pthread
)
# target_link_libraries(${TARGET_NAME}
#     ${glog_LIBRARIES}
#     ${Gflags_LIBRARIES}
#     ${Poco_LIBRARIES}
#     fastrtps
#     fastcdr
#     atomic
# )

