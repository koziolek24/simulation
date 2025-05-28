file(GLOB GUI_SOURCES CONFIGURE_DEPENDS *.cc)



add_library(gui STATIC ${GUI_SOURCES})

# headers live in project-wide include/
target_include_directories(gui
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# link against the SFML targets that were created in third_party/SFML
target_link_libraries(gui
    PUBLIC
        sfml-graphics          # -> pulls sfml-window & sfml-system
)
