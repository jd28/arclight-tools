find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SRC_FILES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon/ZFontIcon/Fonts.qrc
)

if(APPLE)
add_executable(texi MACOSX_BUNDLE
    ${SRC_FILES}
)

set_target_properties(texi
    PROPERTIES
    BUNDLE True
    MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.texi
    MACOSX_BUNDLE_BUNDLE_NAME texi
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
elseif(MSVC)
add_executable(texi WIN32
    ${SRC_FILES}
)
else()
add_executable(texi
    ${SRC_FILES}
)
endif()

target_include_directories(texi PRIVATE
    ../
    ${CMAKE_SOURCE_DIR}/external/rollnw/external
    ${CMAKE_SOURCE_DIR}/external/rollnw/lib
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(texi PRIVATE
    arclight-widgets
    nw
    arclight-external
    Qt6::Widgets
)

if(LINUX)
target_link_libraries(texi PRIVATE
    dl
)
endif()
