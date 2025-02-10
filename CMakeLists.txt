cmake_minimum_required(VERSION 3.0)
project(orange)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)

set(LIB_SRC
    src/log.cpp
    src/util.cpp
)

add_library(orange SHARED ${LIB_SRC})

add_executable(test tests/test.cpp)
add_dependencies(test orange)
target_link_libraries(test orange)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)