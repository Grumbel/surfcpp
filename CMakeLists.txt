# surf - Software surface library
# Copyright (C) 2008-2020 Ingo Ruhnke <grumbel@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.6)
project(surf)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/external/cmake-modules/)

include(GNUInstallDirs)
include(MaximumWarnings)
include(ClangTidy)

find_package(PkgConfig REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
pkg_search_module(EXIF REQUIRED libexif IMPORTED_TARGET)

# Not using find_package(), see:
# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=776832
# find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
pkg_search_module(MAGICKXX REQUIRED Magick++ GraphicsMagick++ IMPORTED_TARGET)

# Not using find_package(), see:
# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=776832
# find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
pkg_search_module(MAGICKXX REQUIRED Magick++ GraphicsMagick++ IMPORTED_TARGET)

function(build_dependencies)
  #if(BUILD_TESTS)
  #  set(BUILD_TESTS OFF)
  #  add_subdirectory(external/uitest/ EXCLUDE_FROM_ALL)
  #endif()

  set(BUILD_TESTS OFF)
  if(NOT TARGET geomcpp)
    add_subdirectory(external/geomcpp/ EXCLUDE_FROM_ALL)
  endif()
  if(NOT TARGET logmich)
    add_subdirectory(external/logmich/ EXCLUDE_FROM_ALL)
  endif()
endfunction()
build_dependencies()

file(GLOB SURF_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  include/surf/*.hpp)

file(GLOB SURF_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/util/*.cpp
  src/plugins/*.cpp
  src/*.cpp)

add_library(surf STATIC ${SURF_SOURCES})
set_target_properties(surf PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)
target_link_libraries(surf PUBLIC
  geomcpp
  logmich
  PNG::PNG
  JPEG::JPEG
  PkgConfig::EXIF
  PkgConfig::MAGICKXX)
target_include_directories(surf INTERFACE src/ include/surf/ include/)
target_include_directories(surf PRIVATE src/ include/surf/)
set_target_properties(surf PROPERTIES PUBLIC_HEADER "${SURF_HEADERS}")

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  file(GLOB TEST_SURF_SOURCES test/*_test.cpp)
  add_executable(test_surf ${TEST_SURF_SOURCES})
  set_target_properties(test_surf PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
  target_compile_options(test_surf PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(test_surf
    surf
    GTest::GTest
    GTest::Main)

  enable_testing()
  add_test(NAME test_surf
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND test_surf)
endif()

install(TARGETS surf
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

# EOF #
