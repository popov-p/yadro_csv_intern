cmake_minimum_required (VERSION 3.8)

project ("csv_intern")

add_executable (csv_intern "main.cpp" "reader.cpp" "utils.cpp" "utils.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET csv_intern PROPERTY CXX_STANDARD 20)
endif()
