file(GLOB VISITORS_SOURCES *.cc)
add_library(visitors STATIC ${VISITORS_SOURCES})
target_link_libraries(visitors
PUBLIC 
ShapeMakerVisitor
)
