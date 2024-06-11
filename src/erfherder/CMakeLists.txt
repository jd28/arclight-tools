find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SRC_FILES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui

    ${CMAKE_SOURCE_DIR}/external/ZFontIcon/ZFontIcon/Fonts.qrc
)

if(APPLE)
add_executable(erfherder MACOSX_BUNDLE
    ${SRC_FILES}
)

set_target_properties(erfherder
    PROPERTIES
    BUNDLE True
    MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.erfherder
    MACOSX_BUNDLE_BUNDLE_NAME erfherder
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
elseif(MSVC)
add_executable(erfherder WIN32
    ${SRC_FILES}
)
else()
add_executable(erfherder
    ${SRC_FILES}
)
endif()

target_include_directories(erfherder PRIVATE
    ../
    ${CMAKE_SOURCE_DIR}/external/rollnw/external
    ${CMAKE_SOURCE_DIR}/external/rollnw/lib
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/src/widgets/
)

target_link_libraries(erfherder PRIVATE
    arclight-widgets
    nw
    arclight-external
    ContainerView
    Qt6::Widgets
)

if(LINUX)
target_link_libraries(erfherder PRIVATE
    dl
)
endif()

if(APPLE)
install(TARGETS erfherder RUNTIME DESTINATION bin BUNDLE DESTINATION bin)
else()
install(TARGETS erfherder)
endif()
