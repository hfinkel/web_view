# Author: Hal Finkel

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(web_view_wx web_view_wx.cpp)

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

install(TARGETS web_view_wx DESTINATION ${LIBRARY_INSTALL_DIR})
install(FILES
  web_view.h
  ${PROJECT_BINARY_DIR}/web_view_wx_export.h DESTINATION ${INCLUDE_INSTALL_DIR}
)

