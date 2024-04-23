find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SRC_FILES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    dialogmodel.h
    dialogmodel.cpp
    dialogview.h
    dialogview.cpp
    dialogview.ui
    dialogtreeview.h
    dialogtreeview.cpp

    ${CMAKE_SOURCE_DIR}/external/ZFontIcon/ZFontIcon/Fonts.qrc
)

if(APPLE)
add_executable(dlg MACOSX_BUNDLE
    ${SRC_FILES}
)

set_target_properties(dlg
    PROPERTIES
    BUNDLE True
    MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.dlg
    MACOSX_BUNDLE_BUNDLE_NAME dlg
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
elseif(MSVC)
add_executable(dlg WIN32
    ${SRC_FILES}
    fontchooserdialog.h fontchooserdialog.cpp fontchooserdialog.ui
)
else()
add_executable(dlg
    ${SRC_FILES}
)
endif()

target_include_directories(dlg PRIVATE
    ../
    ${CMAKE_SOURCE_DIR}/external/rollnw/external
    ${CMAKE_SOURCE_DIR}/external/rollnw/external/sqlite-3.45.2
    ${CMAKE_SOURCE_DIR}/external/rollnw/external/minizip/include
    ${CMAKE_SOURCE_DIR}/external/rollnw/lib
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(dlg PRIVATE
    arclight-widgets
    nw
    sqlite3
    arclight-external
    qtpropertybrowser
    Qt6::Widgets
)

if(LINUX)
target_link_libraries(dlg PRIVATE
    dl
)
endif()
