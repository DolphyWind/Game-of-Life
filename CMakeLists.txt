cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROGRAM_NAME Game-Of-Life)
project(${PROGRAM_NAME} VERSION 1.0.0)
find_package(SFML 2.5.1 REQUIRED COMPONENTS graphics window system)
find_package(TGUI REQUIRED)

file(GLOB SOURCE_FILES "./src/*.cpp")

add_executable(${PROGRAM_NAME} ${SOURCE_FILES})
target_include_directories(${PROGRAM_NAME} PUBLIC include)
target_link_libraries(${PROGRAM_NAME} sfml-graphics sfml-system sfml-window tgui)
