file(GLOB SIMULATION_SOURCES *.cc)
add_library(simulation STATIC ${SIMULATION_SOURCES})
target_link_libraries(simulation
  PUBLIC
    city
)
