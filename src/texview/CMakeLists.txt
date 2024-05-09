find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SRC_FILES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    texuregallerymodel.h
    texuregallerymodel.cpp
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon/ZFontIcon/Fonts.qrc
)

if(APPLE)
add_executable(texview MACOSX_BUNDLE
    ${SRC_FILES}
)

set_target_properties(texview
    PROPERTIES
    BUNDLE True
    MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.texview
    MACOSX_BUNDLE_BUNDLE_NAME texview
    MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
)
elseif(MSVC)
add_executable(texview WIN32
    ${SRC_FILES}
)
else()
add_executable(texview
    ${SRC_FILES}
)
endif()

target_include_directories(texview PRIVATE
    ../
    ${CMAKE_SOURCE_DIR}/external/rollnw/external
    ${CMAKE_SOURCE_DIR}/external/rollnw/external/minizip/include
    ${CMAKE_SOURCE_DIR}/external/rollnw/lib
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(texview PRIVATE
    arclight-widgets
    nw
    arclight-external
    minizip
    Qt6::Widgets
)

if(LINUX)
target_link_libraries(texview PRIVATE
    dl
)
endif()

install(TARGETS texview)
