find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL OpenGLWidgets)

add_library(renderer STATIC
    shader.cpp
    shader.hpp
    model.cpp
    model.hpp
    ModelCache.cpp
    ModelCache.hpp
    TextureCache.cpp
    TextureCache.hpp
    util.cpp
    util.hpp
)

target_compile_definitions(renderer PUBLIC -DGLM_ENABLE_EXPERIMENTAL)

target_include_directories(renderer PRIVATE
    ./
    ${CMAKE_SOURCE_DIR}/external/rollnw/external
    ${CMAKE_SOURCE_DIR}/external/rollnw/external/sqlite-3.45.2
    ${CMAKE_SOURCE_DIR}/external/rollnw/external/minizip/include
    ${CMAKE_SOURCE_DIR}/external/rollnw/lib
    ${CMAKE_SOURCE_DIR}/external/ZFontIcon
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(renderer PRIVATE
    arclight-widgets
    arclight-external
    nw
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    nw
)
