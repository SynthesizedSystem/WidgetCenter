# CMakeList.txt : CMake project for WidgetCenter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("WidgetCenter")

set(SOURCES
  "src/main.cpp"
  "src/apps/base/application.h"
  "src/apps/base/application.cpp"
  "src/win32_api/win32window.h"
  "src/win32_api/win32window.cpp"
  "src/apps/SandboxApplication.h"
  "src/apps/SandboxApplication.cpp"
  "src/core/logger.h"
  "src/core/logger.cpp")

# Add source to this project's executable.
add_executable (WidgetCenter ${SOURCES})
target_include_directories(WidgetCenter PRIVATE "src/")
target_precompile_headers(WidgetCenter PRIVATE "src/stdafx.h")
target_link_libraries(WidgetCenter d3d11 dxgi d3dcompiler winmm dxguid)

if(TRACE)
  target_compile_definitions(WidgetCenter PRIVATE "_TRACE")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WidgetCenter PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
