cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(PROJECT_NAME "digilivolo")
project(${PROJECT_NAME} C)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -std=gnu11 -flto -ffunction-sections -fdata-sections -ffat-lto-objects -Wall -Wl,--warn-common -Wl,--gc-sections")
endif()
set(HIDAPI_WITH_LIBUSB FALSE)
set(BUILD_SHARED_LIBS FALSE) # HIDAPI as static library on all platforms

if(POLICY CMP0074)
    # allow using hidapi_ROOT if CMake supports it
    cmake_policy(SET CMP0074 NEW)
endif()

if(POLICY CMP0115)
    cmake_policy(SET CMP0115 NEW)
endif()

# Get version from git
execute_process(
    COMMAND git describe --tags
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Project: ${PROJECT_NAME} ${GIT_VERSION}")

configure_file(src/git_version.h.in src/git_version.h @ONLY)
add_executable(${PROJECT_NAME} src/args.c src/digilivolo.c src/usb_func.c)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/src")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(argp)
message(STATUS "DEBUG ARGP: ARGP_FOUND = ${ARGP_FOUND}, ARGP_LIBRARIES = ${ARGP_LIBRARIES}, ARGP_INCLUDE_PATH = ${ARGP_INCLUDE_PATH}")

if(NOT ARGP_FOUND)
    add_subdirectory(lib/argp-standalone)
    target_link_libraries(${PROJECT_NAME} argp-standalone)
endif()

if(USE_SYSTEM_HIDAPI)
    message(STATUS "Finding library hidapi")
    find_package(HIDAPI 0.13 REQUIRED)
    target_link_libraries(${PROJECT_NAME} HIDAPI::hidapi)
else()
    add_subdirectory(lib/hidapi)
    message(STATUS "hidapi will be built from sources")
    target_link_libraries(${PROJECT_NAME} hidapi::hidapi)
    message(STATUS "Using HIDAPI: ${hidapi_VERSION}")
endif()

# target_link_libraries(${PROJECT_NAME} HIDAPI::hidapi)

# Strip binary for release builds
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_STRIP} $<TARGET_FILE:${PROJECT_NAME}>
  VERBATIM
)
