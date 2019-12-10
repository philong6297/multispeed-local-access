cmake_minimum_required(VERSION 3.15)
project(msla LANGUAGES CXX)

set(LONGLP_PROJECT_DIR "${PROJECT_SOURCE_DIR}")
set(LONGLP_PROJECT_SRC_DIR "${LONGLP_PROJECT_DIR}/src")
set(LONGLP_PROJECT_EXTERNAL_DIR "${LONGLP_PROJECT_DIR}/external")
set(LONGLP_PROJECT_DATA_DIR "${LONGLP_PROJECT_DIR}/data")
set(LONGLP_PROJECT_OUTPUT_DIR "${PROJECT_BINARY_DIR}")

add_subdirectory(tools/cmake)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 5.13 COMPONENTS Gui Core Widgets REQUIRED)
longlp_make_exec(
  TARGET_NAME ${PROJECT_NAME}
  USE_DEFAULT
  SOURCES
  #[[]] PRIVATE
  #[[]]   src/main.cpp
  #[[]]   src/ui/node.cpp src/ui/node.hpp
  #[[]]   src/ui/graph_widget.cpp src/ui/graph_widget.hpp
  #[[]]   src/ui/edge.cpp src/ui/edge.hpp
  #[[]]   src/msla/defs.hpp
  #[[]]   src/msla/solver.cpp src/msla/solver.hpp
  #[[]]   src/data_setup.hpp src/data_setup.cpp
  LINK_LIBRARIES
  #[[]] PRIVATE
  #[[]]   Qt5::Gui Qt5::Core Qt5::Widgets
  INCLUDE_DIRECTORIES
  #[[]]  PRIVATE
  #[[]]   src
)
