cmake_minimum_required (VERSION 2.6)

project (PrPa)

set(
  SOURCES
  src/main.cpp
  src/img_processing.cpp
  )

find_package(OpenCV REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/includes/)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-W -Wall -ltbb -lrt -std=c++11")

add_executable(
  prpa
  ${SOURCES}
  )

target_link_libraries(
  prpa
  ${OpenCV_LIBS}
  )
