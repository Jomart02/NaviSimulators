file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS include/*.h src/*.cpp ui/*.ui)

add_library(ToolBoxWidget STATIC ${SRCS})
navi_set_uic_paths(ToolBoxWidget ${SRCS})
target_include_directories(ToolBoxWidget PUBLIC include)
target_link_libraries(ToolBoxWidget PUBLIC Qt6::Widgets)
