set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/src)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/base BASE_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/client CLIENT_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/ds DS_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/lib LIB_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/server SERVER_FILES)

add_executable(server ${BASE_FILES} ${DS_FILES} ${LIB_FILES} ${SERVER_FILES})
add_executable(client ${BASE_FILES} ${DS_FILES} ${LIB_FILES} ${CLIENT_FILES})
