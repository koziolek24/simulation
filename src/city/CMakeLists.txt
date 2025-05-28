file(GLOB CITY_SOURCES *.cc)
add_library(city STATIC ${CITY_SOURCES})
target_link_libraries(city 
PUBLIC 
agents
)
