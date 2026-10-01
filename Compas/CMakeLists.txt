navi_add_plugin(CompassPlugin
    LIBS nmeabuilder
    LANGS en ru)

if(BUILD_TESTING)
    navi_add_test(CompassTest CompassPlugin tests/CompassTest.cpp)
endif()
