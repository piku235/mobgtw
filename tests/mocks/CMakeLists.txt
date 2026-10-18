find_package(PkgConfig REQUIRED)

add_library(mocks OBJECT
  MockMqttMobilusActor.cpp
  MockMqttMobilusActorImpl.cpp
)

target_include_directories(mocks
  PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(mocks
  PRIVATE
    libmosquitto_static
    mobgtw
)
