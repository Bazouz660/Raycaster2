cmake_minimum_required(VERSION 3.15)
project(sfml_conan_raycaster
  LANGUAGES CXX
  VERSION 1.0
)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Set Conan toolchain
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(sfml REQUIRED)
find_package(imgui REQUIRED)

include_directories(include)

# Add your source files here
set(SOURCES
    src/main.cpp
    src/Raycaster.cpp
    src/Player.cpp
    src/Map.cpp
    src/Game.cpp
    src/Clock.cpp
    src/TextureMap.cpp
    src/AEntity.cpp
    src/StaticEntity.cpp
    src/ItemEntity.cpp
    src/EnemyEntity.cpp
    src/imgui-SFML.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} sfml::sfml imgui::imgui)

# Copy executable to root directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_BINARY_DIR}
)
