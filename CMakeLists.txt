cmake_minimum_required(VERSION 3.22)
project(zpr25l LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/Freesans.ttf")

# Enable AddressSanitizer if requested
option(ENABLE_ASAN "Enable AddressSanitizer" ON)

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address -fno-omit-frame-pointer)
endif()

# Also add strict warnings
add_compile_options(
    -Wall -Wextra -Wpedantic -Werror
    -Wnull-dereference -Wdouble-promotion
    -Wshadow -Wconversion -Wsign-conversion -Wformat=2
    -g
)

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)

add_subdirectory(third_party/SFML)
add_subdirectory(third_party/googletest)

# Collect all source files except main.cc
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cc
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
list(REMOVE_ITEM SRC_FILES "${PROJECT_SOURCE_DIR}/src/main.cc")

add_library(zpr25l_lib ${SRC_FILES})
target_include_directories(zpr25l_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)
target_link_libraries(zpr25l_lib PUBLIC  # <-- CHANGE PRIVATE TO PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    ${OPENGL_gl_LIBRARY}
    ${X11_LIBRARIES}
    ${FREETYPE_LIBRARIES}
)

add_executable(zpr25l_sim src/main.cc)
target_link_libraries(zpr25l_sim PRIVATE zpr25l_lib)

enable_testing()
add_subdirectory(tests)

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cc
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/include/*.h
        ${PROJECT_SOURCE_DIR}/tests/*.cc
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Formatting code with clang-format"
        VERBATIM
    )
else()
    message(STATUS "clang-format not found, format target not available")
endif()

install(FILES ${APP_RESOURCES}
        DESTINATION share/${PROJECT_NAME}/fonts)

install(TARGETS zpr25l_sim
        RUNTIME DESTINATION bin)
