set(LIB_SRC
    log.cpp
    util.cpp
    config.cpp
)

add_library(orange SHARED ${LIB_SRC})
target_link_libraries(orange yaml-cpp)