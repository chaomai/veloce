set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/third_party/gtest/include)
include_directories(${PROJECT_SOURCE_DIR}/test)

include_directories(${PROJECT_SOURCE_DIR}/src)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/ds DS_FILES)

aux_source_directory(${PROJECT_SOURCE_DIR}/test TEST_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/test/ds TEST_DS_FILES)

add_executable(unit_test ${TEST_FILES} ${TEST_DS_FILES} ${DS_FILES})
target_link_libraries(unit_test gtest)
