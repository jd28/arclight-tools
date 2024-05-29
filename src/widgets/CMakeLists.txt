# Enable policy to run automoc on generated files.
if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(arclight-widgets STATIC
    AbstractTreeModel.cpp
    AbstractTreeModel.hpp

    ArclightView.h
    ArcLightView.cpp

    TlkSelector/tlkselector.h
    TlkSelector/tlkselector.cpp
    TlkSelector/tlkselector.ui

    util/restypeicons.cpp
    checkboxdelegate.h
    checkboxdelegate.cpp
    projectview.h
    projectview.cpp
    filesystemview.h
    filesystemview.cpp
    proxymodels.h
    proxymodels.cpp
)

add_subdirectory(AreaView)
add_subdirectory(ContainerView)
add_subdirectory(CreatureView)
add_subdirectory(DialogView)
add_subdirectory(LanguageMenu)
add_subdirectory(qtpropertybrowser)
add_subdirectory(renderer)
add_subdirectory(TlkView)

target_include_directories(arclight-widgets PRIVATE
    ${CMAKE_SOURCE_DIR}/external/rollnw/external
    ${CMAKE_SOURCE_DIR}/external/rollnw/external/sqlite-3.45.2
    ${CMAKE_SOURCE_DIR}/external/rollnw/external/minizip/include
    ${CMAKE_SOURCE_DIR}/external/rollnw/lib
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(arclight-widgets PRIVATE
    nw
    arclight-external
    Qt6::Widgets
)
