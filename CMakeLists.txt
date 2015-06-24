project(conv_test)

cmake_minimum_required(VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(test SOURCES)

add_executable(${PROJECT_NAME} ${SOURCES})
