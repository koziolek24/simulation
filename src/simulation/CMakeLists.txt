file(GLOB SIMULATION_SOURCES *.cc)
add_library(engine STATIC ${SIMULATION_SOURCES})
target_link_libraries(engine
  PUBLIC
    city
)
