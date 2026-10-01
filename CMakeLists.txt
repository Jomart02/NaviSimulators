cmake_minimum_required(VERSION 3.21)

project(NaviSimulators VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(MSVC)
    add_compile_definitions(_USE_MATH_DEFINES) 
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools Test)


set(NAVI_OUTPUT_DIR ${CMAKE_BINARY_DIR}/output CACHE PATH "Where plugins and executables are built")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${NAVI_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${NAVI_OUTPUT_DIR})

include(CTest)
include(cmake/NaviPlugin.cmake)


add_subdirectory(BaseNaviWidget)
add_subdirectory(Editable-Table-Delegate)
add_subdirectory(ToolBoxWidget)

add_subdirectory(SNS)
add_subdirectory(Compas)
add_subdirectory(Weather)
add_subdirectory(EchoSounder)
add_subdirectory(LagRelative)
add_subdirectory(LagAbs)
add_subdirectory(AIS)
add_subdirectory(ARPA)

add_subdirectory(tools/PluginHost)

# cmake --build <dir> --target plugins_zip 
get_property(NAVI_PLUGINS GLOBAL PROPERTY NAVI_PLUGINS)
foreach(T BaseNaviWidgetLib ${NAVI_PLUGINS})
    list(APPEND ZIP_FILES $<TARGET_FILE_NAME:${T}>)
endforeach()
add_custom_target(plugins_zip
    COMMAND ${CMAKE_COMMAND} -E tar cf ${CMAKE_BINARY_DIR}/plugins.zip --format=zip -- ${ZIP_FILES}
    WORKING_DIRECTORY ${NAVI_OUTPUT_DIR}
    DEPENDS BaseNaviWidgetLib ${NAVI_PLUGINS}
    COMMENT "Creating ${CMAKE_BINARY_DIR}/plugins.zip")
