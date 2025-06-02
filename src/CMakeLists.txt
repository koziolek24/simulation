add_subdirectory(agents)
add_subdirectory(city)
add_subdirectory(disease)
add_subdirectory(simulation)
add_subdirectory(utils)
add_subdirectory(gui)
add_subdirectory(visitors)
add_executable(zpr25l_sim main.cc)
target_link_libraries(zpr25l_sim
    agents
    city
    disease
    utils
    simulation
    gui
    visitors
)
