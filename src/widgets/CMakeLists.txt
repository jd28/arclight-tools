find_package(Qt5 COMPONENTS Widgets Multimedia MultimediaWidgets REQUIRED)

add_library(arclight-widgets STATIC
    ContainerModel.cpp
    ContainerModel.hpp
    ContainerWidget.cpp
    ContainerWidget.hpp
    ContainerWidget.ui

    LanguageMenu.cpp
    LanguageMenu.hpp
    TlkSelector/tlkselector.h
    TlkSelector/tlkselector.cpp
    TlkSelector/tlkselector.ui

    TlkModel.cpp
    TlkModel.hpp
    TlkView.cpp
    TlkView.hpp
    TlkView.ui

    util/restypeicons.cpp
)

target_include_directories(arclight-widgets PRIVATE
    ${CMAKE_SOURCE_DIR}/external/libnw/external
    ${CMAKE_SOURCE_DIR}/external/libnw/lib
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(arclight-widgets PRIVATE
    nw
    arclight-external
    Qt5::Widgets
)
