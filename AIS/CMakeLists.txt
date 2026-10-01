navi_add_plugin(AISPlugin
    LIBS ToolBoxWidget CoordinateEdit
    INCLUDE_DIRS
        include/BasePage
        include/ClassA
        include/ClassB
        include/SAR
        include/ATON
        include/BaseStation)

if(BUILD_TESTING)
    navi_add_test(AisCodecTest AISPlugin tests/AisCodecTest.cpp)
    navi_add_test(AisPluginTest AISPlugin tests/AisPluginTest.cpp)
endif()
