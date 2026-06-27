cmake_minimum_required(VERSION 3.20)
project(perception_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /permissive-)
endif()

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(imgui nlohmann_json)

set(IMGUI_DIR ${imgui_SOURCE_DIR})

add_library(imgui_lib STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx11.cpp
)

target_include_directories(imgui_lib PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui_lib PUBLIC d3d11 dxgi)

add_executable(perception_gui
    src/main.cpp
    src/pcx/renderer.cpp
    src/pcx/input.cpp
    src/pcx/base64.cpp
    src/pcx/file_util.cpp
    src/pcx/gui/hotkeys.cpp
    src/pcx/gui/elements_impl.cpp
    src/pcx/gui/containers.cpp
    src/pcx/gui/config_system.cpp
    src/pcx/gui/theme_system.cpp
    src/pcx/gui/notifications.cpp
    src/pcx/gui/widgets.cpp
    src/pcx/gui/identity.cpp
    src/pcx/gui/registration.cpp
    src/pcx/gui/gui.cpp
)

target_include_directories(perception_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(perception_gui PRIVATE
    imgui_lib
    nlohmann_json::nlohmann_json
    d3d11
    dxgi
    d3dcompiler
)

if(WIN32)
    target_link_libraries(perception_gui PRIVATE imm32)
endif()
