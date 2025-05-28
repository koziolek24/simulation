file(GLOB AGENTS_SOURCES *.cc)
add_library(agents STATIC ${AGENTS_SOURCES})
target_link_libraries(agents 
PUBLIC 
city
)

