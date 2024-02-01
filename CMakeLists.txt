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
find_package(PkgConfig REQUIRED)
find_package(fastrtps REQUIRED)
find_package(fastcdr REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation)
find_package(glog REQUIRED)
find_package(Protobuf REQUIRED)
# find_package(Python REQUIRED COMPONENTS Interpreter Development)
pkg_check_modules(Gflags REQUIRED gflags)


# set(Python_VERSION "${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")
set(Python_VERSION "${Python_VERSION}")
set(CYBER_PYTHON_PATH "lib/python${Python_VERSION}/site-packages/cyber/python")
set(CYBER_PYTHON_INTERNAL_PATH "${CYBER_PYTHON_PATH}/internal")
set(CYBER_PYTHON_PROTOBUF_PATH "lib/python${Python_VERSION}/site-packages/cyber/proto")
set(CYBER_MSGS_CACHE ON CACHE BOOL "generate cyber proto msgs flag.")
# set(CYBER_PYTHON_LIB "/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu/libpython3.6.so")

if(NOT DEFINED glog_LIBRARIES)
  set(glog_LIBRARIES "glog")
endif()

if(NOT DEFINED Uuid_LIBRARIES)
  set(Uuid_LIBRARIES "uuid")
endif()

set(DEPENCENCY_INCLUDE_DIRS
  ${fastcdr_INCLUDE_DIR}
  ${fastrtps_INCLUDE_DIR}
  ${Protobuf_INCLUDE_DIRS}
  ${Python_INCLUDE_DIRS}
  ${Uuid_INCLUDE_DIRS}
  ${glog_INCLUDE_DIRS}
  ${Gflags_INCLUDE_DIRS}
  ${NlohmannJson_INCLUDE_DIRS}
)

set(DEPENCENCY_LIB_DIRS
  ${fastcdr_LIB_DIR}
  ${fastrtps_LIB_DIR}
  ${Protobuf_LIBRARIES_DIRS}
  ${Python_LIBRARIES_DIRS}
  ${Uuid_LIBRARIES_DIRS}
  ${glog_LIBRARY_DIRS}
  ${Gflags_LIBRARY_DIRS}
  ${NlohmannJson_LIBRARY_DIRS}
)

set(DEPENCENCY_LIBS
  ${glog_LIBRARIES}
  ${Gflags_LIBRARIES}
)

message("### include_directories: ${CMAKE_CURRENT_SOURCE_DIR}")

# include_directories(
#   ${CMAKE_CURRENT_SOURCE_DIR}
# )
include_directories(/usr/local/workspace/github/CyberRT-9.0.0/cyber/transport/shm)
# include_directories(${PROJECT_SOURCE_DIR})
# link_directories(${CMAKE_CURRENT_SOURCE_DIR})


if(${CYBER_MSGS_CACHE})
  set(CYBER_MSGS_CACHE OFF CACHE BOOL "generate cyber proto msgs flag." FORCE)
  FOREACH(FIL ${CYBER_PROTO_FILES})
    message("### generate proto file: ${FIL}")
    GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FIL})
    string(REGEX REPLACE "(.+)\\${FILE_NAME}.*" "\\1" FILE_PATH ${FIL})
    EXECUTE_PROCESS(
      COMMAND
      ${PROTOBUF_PROTOC_EXECUTABLE}
      -I${CMAKE_CURRENT_SOURCE_DIR}
      --cpp_out=${CMAKE_CURRENT_SOURCE_DIR}
      --python_out=${CMAKE_CURRENT_SOURCE_DIR}
      ${FIL}
    )
  ENDFOREACH()
endif(${CYBER_MSGS_CACHE})

file(GLOB SHM_COM_SRCS
    "cyber/transport/shm/*.cc"
)

# list(FILTER SHM_COM_SRCS EXCLUDE REGEX .*test[.]cc)

# add_library(${TARGET_NAME} SHARED
#     ${CYBER_SRCS}
# )

add_executable(${TARGET_NAME} cyber/transport/shm/shm_test.cc ${SHM_COM_SRCS})
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

install(DIRECTORY ${CMAKE_BINARY_DIR}/${CYBER_PYTHON_PROTOBUF_PATH}/
  DESTINATION ${CYBER_PYTHON_PROTOBUF_PATH}
)

# install
install(TARGETS ${TARGET_NAME}
    EXPORT ${TARGET_NAME}-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

