cmake_minimum_required(VERSION 3.2)

project(veloce)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Weverything -Wno-c++11-extensions -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -pthread")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -O2 -g")

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME ./unit_test COMMAND ./unit_test)
add_custom_target(
  check
  ${CMAKE_CTEST_COMMAND} --verbose
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  VERBATIM
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(
    doc
    ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    VERBATIM
  )
endif(DOXYGEN_FOUND)
