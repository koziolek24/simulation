file(GLOB UTILS_SOURCES *.cc)
add_library(utils STATIC ${UTILS_SOURCES})
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)

