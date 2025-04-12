cmake_minimum_required(VERSION 3.10)
project(FractalVisualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options(/bigobj)
endif()

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
    "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/Dependencies/
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui
    ${CMAKE_SOURCE_DIR}/Dependencies/imgui/backends
)
link_directories(${CMAKE_SOURCE_DIR}/Dependencies/lib)

find_package(OpenGL REQUIRED)

add_executable(FractalVisualizer 
    src/main.cpp
    "Dependencies/glad.c"
    ${IMGUI_SOURCES}
 "src/controls.cpp" "src/shader.cpp" "src/shader.h" "src/controls.h" "src/state.h" "src/gui.cpp" "src/gui.h" "src/complexParser.h" "src/complexParser.cpp" "resources/iconViewer.rc")

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})

target_link_libraries(FractalVisualizer PRIVATE glfw3 opengl32)