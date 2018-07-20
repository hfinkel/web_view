#                Copyright (C) 2018, UChicago Argonne, LLC
#                           All Rights Reserved
#
#                         Software Name: web_view
#                     By: Argonne National Laboratory
#                           OPEN SOURCE LICENSE
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#   1. Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   3. Neither the name of the copyright holder nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# *****************************************************************************
#                                DISCLAIMER
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# *****************************************************************************

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

add_executable(test1 tests/test1.cpp)
target_link_libraries(test1 web_view_wx)

install(TARGETS web_view_wx DESTINATION ${LIB_INSTALL_DIR})
install(FILES
  web_view.h
  ${PROJECT_BINARY_DIR}/web_view_wx_export.h DESTINATION ${INCLUDE_INSTALL_DIR}
)

