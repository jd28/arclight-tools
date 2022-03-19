find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library(arclight-external STATIC
    fzy/match.c
    ZFontIcon/ZFontIcon/ZFontIcon.cpp
)

target_link_libraries(arclight-external PRIVATE
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)

add_subdirectory(libnw)
