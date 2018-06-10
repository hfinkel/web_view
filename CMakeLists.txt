# Author: Hal Finkel

cmake_minimum_required(VERSION 3.10)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(web_view_wx web_view_wx.cpp)
target_include_directories(web_view_wx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(web_view_wx PUBLIC ${CMAKE_BINARY_DIR})

find_package(wxWidgets REQUIRED net webview core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(web_view_wx ${wxWidgets_LIBRARIES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")
endif()

include(GenerateExportHeader)
generate_export_header(web_view_wx)

if(INCLUDE_INSTALL_DIR)
else()
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
endif()
if(LIB_INSTALL_DIR)
else()
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
endif()

add_executable(test test/test.cpp)
target_link_libraries(test web_view_wx)

install(TARGETS web_view_wx DESTINATION ${LIB_INSTALL_DIR})
install(FILES
  web_view.h
  ${PROJECT_BINARY_DIR}/web_view_wx_export.h DESTINATION ${INCLUDE_INSTALL_DIR}
)

