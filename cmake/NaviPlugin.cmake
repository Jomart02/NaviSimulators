# Каталоги .ui
function(navi_set_uic_paths TARGET)
    foreach(F ${ARGN})
        if(F MATCHES "[.]ui$")
            get_filename_component(D ${F} DIRECTORY)
            list(APPEND DIRS ${D})
        endif()
    endforeach()
    list(REMOVE_DUPLICATES DIRS)
    set_target_properties(${TARGET} PROPERTIES AUTOUIC_SEARCH_PATHS "${DIRS}")
endfunction()

# navi_add_plugin(<Name> [LIBS <targets...>] [LANGS <en ru ...>] [INCLUDE_DIRS <dirs...>])
#
# Собирает плагин из include/*.h, src/*.cpp и ui/*.ui текущего каталога:
#   <Name>_obj - OBJECT-библиотека
#   <Name>     - SHARED-плагин  ( Lazuli)
function(navi_add_plugin NAME)
    cmake_parse_arguments(P "" "" "LIBS;LANGS;INCLUDE_DIRS" ${ARGN})

    file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS include/*.h src/*.cpp ui/*.ui)

    add_library(${NAME}_obj OBJECT ${SRCS})
    navi_set_uic_paths(${NAME}_obj ${SRCS})
    set_target_properties(${NAME}_obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_include_directories(${NAME}_obj PUBLIC include ${P_INCLUDE_DIRS})
    target_compile_definitions(${NAME}_obj PUBLIC PROJECT_NAME="${NAME}")
    target_link_libraries(${NAME}_obj PUBLIC Qt6::Widgets BaseNaviWidgetLib ${P_LIBS})

    if(P_LANGS)
        foreach(L ${P_LANGS})
            list(APPEND TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/translations/${NAME}_${L}.ts)
        endforeach()
        qt_add_lupdate(${NAME}_obj TS_FILES ${TS_FILES} NO_GLOBAL_TARGET)
        qt_add_lrelease(${NAME}_obj TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES)
        foreach(QM ${QM_FILES})
            get_filename_component(QM_NAME ${QM} NAME_WE)       # <Name>_<lang>
            string(REGEX REPLACE ".*_" "" LANG ${QM_NAME})
            set_source_files_properties(${QM} PROPERTIES QT_RESOURCE_ALIAS ${LANG}${NAME})
        endforeach()
        qt_add_resources(${NAME}_obj ${NAME}_translations
            PREFIX /translations BASE ${CMAKE_CURRENT_BINARY_DIR} FILES ${QM_FILES})
    endif()

    add_library(${NAME} SHARED $<TARGET_OBJECTS:${NAME}_obj>)
    target_link_libraries(${NAME} PRIVATE ${NAME}_obj)

    set_property(GLOBAL APPEND PROPERTY NAVI_PLUGINS ${NAME})
endfunction()

# navi_add_test(<Name> <Plugin> <sources...>)
function(navi_add_test NAME PLUGIN)
    add_executable(${NAME} ${ARGN})
    target_link_libraries(${NAME} PRIVATE Qt6::Test ${PLUGIN}_obj)
    add_test(NAME ${NAME} COMMAND ${NAME})
    set_tests_properties(${NAME} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endfunction()
