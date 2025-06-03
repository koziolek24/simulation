cmake_minimum_required(VERSION 3.22)
project(zpr25l LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── AddressSanitizer (optional) ───
option(ENABLE_ASAN "Enable AddressSanitizer" ON)
if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options    (-fsanitize=address -fno-omit-frame-pointer)
endif()

# ─── Strict warnings – applied **only** to our code ───
set(PROJ_STRICT_WARN
    -Wall -Wextra -Wpedantic -Werror
    -Wnull-dereference -Wdouble-promotion
    -Wshadow -Wconversion -Wsign-conversion -Wformat=2
    -g
)

# ─── System packages ───
find_package(OpenGL   REQUIRED)
find_package(X11      REQUIRED)
find_package(Freetype REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system CONFIG REQUIRED)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import numpy"
    RESULT_VARIABLE HAS_NUMPY
)
if(NOT HAS_NUMPY EQUAL 0)
    message(FATAL_ERROR "NumPy not found in ${Python3_EXECUTABLE}. Please install it.")
endif()

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import matplotlib"
    RESULT_VARIABLE HAS_MATPLOTLIB
)
if(NOT HAS_MATPLOTLIB EQUAL 0)
    message(FATAL_ERROR "matplotlib not found in Python3 (${Python3_EXECUTABLE}). Please install it.")
else()
    message(STATUS "matplotlib found for Python3 (${Python3_EXECUTABLE})")
endif()

execute_process(
    COMMAND ${Python3_EXECUTABLE} -c "import site; print(site.getsitepackages()[0])"
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_compile_definitions(
    PYTHON_EXECUTABLE="${Python3_EXECUTABLE}"
    PYTHON_SITE="${PYTHON_SITE_PACKAGES}"
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/matplotlib-cpp
    ${OPENGL_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
)

# ─── GoogleTest via FetchContent ───
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true  # Silence CMP0135 warning
)
FetchContent_MakeAvailable(googletest)

# ─── Project sources ───
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
     ${PROJECT_SOURCE_DIR}/src/*.cc
     ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cc)

# ─── Library target ───
add_library(zpr25l_lib ${PROJECT_SOURCES})

target_include_directories(zpr25l_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
)

target_compile_options(zpr25l_lib PRIVATE ${PROJ_STRICT_WARN})

target_link_libraries(zpr25l_lib PUBLIC
    sfml-graphics sfml-window sfml-system
    ${OPENGL_gl_LIBRARY} ${X11_LIBRARIES} ${FREETYPE_LIBRARIES}
    Python3::Python Python3::NumPy
)

# ─── Main executable ───
add_executable(zpr25l_sim src/main.cc)

target_compile_options(zpr25l_sim PRIVATE ${PROJ_STRICT_WARN})
target_link_libraries(zpr25l_sim PRIVATE zpr25l_lib)

# ─── Tests ───
enable_testing()
add_subdirectory(tests)

# ─── clang‑format helper ───
find_program(CLANG_FORMAT NAMES clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE
         ${PROJECT_SOURCE_DIR}/src/*.[ch]*
         ${PROJECT_SOURCE_DIR}/include/*.[ch]*
         ${PROJECT_SOURCE_DIR}/tests/*.[ch]*)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE}
        COMMENT "Formatting sources with clang-format")
else()
    message(STATUS "clang-format not found – 'make format' disabled.")
endif()

# ─── Installation ───
install(TARGETS zpr25l_sim RUNTIME DESTINATION bin)
install(DIRECTORY resources/fonts DESTINATION share/${PROJECT_NAME}/fonts)
