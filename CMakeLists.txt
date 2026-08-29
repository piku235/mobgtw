cmake_minimum_required(VERSION 3.28)
project(mobgtw VERSION 0.10.0)

cmake_policy(SET CMP0135 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CMakeDependentOption)
include(CTest)
include(FetchContent)

cmake_dependent_option(MOBGTW_BUILD_TESTS "Build mobgtw tests" ON "PROJECT_IS_TOP_LEVEL AND BUILD_TESTING AND NOT CMAKE_CROSSCOMPILING" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED IMPORTED_TARGET libcrypto)

set(EXPECTED_BUILD_TESTS OFF)
set(EXPECTED_BUILD_PACKAGE OFF)

FetchContent_Declare(
  tl_expected
  URL https://github.com/TartanLlama/expected/archive/refs/tags/v1.3.1.zip
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(tl_expected)

set(WITH_TLS_PSK OFF)
set(WITH_TESTS OFF)
set(WITH_SOCKS OFF)
set(WITH_WEBSOCKETS OFF)
set(WITH_SRV OFF)
set(WITH_STATIC_LIBRARIES ON)
set(WITH_PIC ON)
set(WITH_CLIENTS OFF)
set(WITH_BROKER OFF)
set(WITH_APPS OFF)
set(WITH_PLUGINS OFF)
set(DOCUMENTATION OFF)

FetchContent_Declare(
  mosquitto
  URL https://github.com/eclipse-mosquitto/mosquitto/archive/refs/tags/v2.0.22.zip
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(mosquitto)
target_include_directories(libmosquitto_static INTERFACE ${mosquitto_SOURCE_DIR}/lib)

add_subdirectory(proto)
add_subdirectory(src/crypto)
add_subdirectory(third_party/protobuf)

if(MOBGTW_BUILD_TESTS)
  add_subdirectory(tests)
endif()

add_library(common_compiler_warnings INTERFACE)

target_compile_options(common_compiler_warnings
  INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wsign-conversion
)

add_library(mobgtw
  src/io/SelectEventLoop.cpp
  src/ExponentialBackoff.cpp
  src/ProtoUtils.cpp
  src/ClientId.cpp
  src/Envelope.cpp
  src/SelectCondition.cpp
  src/MqttDsn.cpp
  src/MqttMobilusGtwClient.cpp
  src/MqttMobilusGtwClientImpl.cpp
)

target_link_libraries(mobgtw
  PUBLIC
    expected
    protos
  PRIVATE
    common_compiler_warnings
    PkgConfig::OPENSSL
    libmosquitto_static
    crypto
)

target_include_directories(mobgtw
  PUBLIC
    include
  PRIVATE
    src
)
