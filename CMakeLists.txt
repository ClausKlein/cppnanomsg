#
#   Copyright (c) 2013 250bpm s.r.o.
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom
#   the Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included
#   in all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
#   IN THE SOFTWARE.
#

cmake_minimum_required(VERSION 3.19...3.21)

project(cppnanomsg LANGUAGES CXX)

# config setting:
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# TODO: use modern cmake and do not set global cmake variables! CK
#
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Werror)
endif()

# ---- dependency ----
#
set(nanomsg_DIR /usr/local/lib/cmake/nanomsg-1.1.5)
find_package(nanomsg 1.1.5 REQUIRED)
add_library(nanomsg::nanomsg ALIAS nanomsg) # FIXME: add missing namespaced alias! CK

# ---- Create header only library ----

add_library(${PROJECT_NAME} INTERFACE include/nanomsg/nn.hpp)
target_link_libraries(${PROJECT_NAME} INTERFACE nanomsg::nanomsg)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD})
target_include_directories(
  ${PROJECT_NAME} BEFORE INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
)
# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${PROJECT_NAME} INTERFACE "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

add_library(nanomsg::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# ---- The test ----

enable_testing()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(binding binding.cpp)
target_link_libraries(binding nanomsg::${PROJECT_NAME})

add_test(NAME test COMMAND binding)

# There's only a single header file to install.
# TODO: export cmake config package setting too! CK

install(FILES include/nanomsg/nn.hpp DESTINATION include/nanomsg)
