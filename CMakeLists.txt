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
project(surf VERSION 0.0.0)

set(TINYCMMC_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/external/cmake-modules/modules/")
find_package(tinycmmc CONFIG)
message(STATUS "tinycmmc module path: ${TINYCMMC_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${TINYCMMC_MODULE_PATH})

include(GNUInstallDirs)
include(MaximumWarnings)
include(ClangTidy)

find_package(PkgConfig REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
pkg_search_module(EXIF libexif IMPORTED_TARGET)

# Not using find_package(), see:
# https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=776832
# find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
pkg_search_module(MAGICKXX Magick++ GraphicsMagick++ IMPORTED_TARGET)

function(build_dependencies)
  #if(BUILD_TESTS)
  #  set(BUILD_TESTS OFF)
  #  add_subdirectory(external/uitest/ EXCLUDE_FROM_ALL)
  #endif()

  set(BUILD_TESTS OFF)

  find_package(geom QUIET)
  if(NOT TARGET geom::geom)
    add_subdirectory(external/geomcpp/ EXCLUDE_FROM_ALL)
  endif()

  find_package(logmich QUIET)
  if(NOT TARGET logmich::logmich)
    add_subdirectory(external/logmich/ EXCLUDE_FROM_ALL)
  endif()
endfunction()
build_dependencies()

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
  geom::geom
  logmich::logmich
  PNG::PNG
  JPEG::JPEG)
target_include_directories(surf INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/surf>
  )
target_include_directories(surf PRIVATE
  include
  include/surf
  src)

if(MAGICKXX_FOUND)
  target_link_libraries(surf PUBLIC
    PkgConfig::MAGICKXX)
  target_compile_definitions(surf PUBLIC "HAVE_MAGICKXX")
endif()

if(EXIF_FOUND)
  target_link_libraries(surf PUBLIC
    PkgConfig::EXIF)
  target_compile_definitions(surf PUBLIC "HAVE_EXIF")
endif()

if(BUILD_TESTS)
  find_package(GTest REQUIRED)

  pkg_search_module(SDL2 REQUIRED sdl2 IMPORTED_TARGET)

  file(GLOB TEST_SURF_SOURCES test/*_test.cpp)
  add_executable(test_surf ${TEST_SURF_SOURCES})
  set_target_properties(test_surf PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
  target_compile_options(test_surf PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(test_surf
    surf
    PkgConfig::SDL2
    GTest::GTest
    GTest::Main)

  enable_testing()
  add_test(NAME test_surf
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMAND test_surf)
endif()

if(BUILD_BENCHMARKS)
  find_package(benchmark REQUIRED)

  # build benchmarks
  file(GLOB BENCHMARK_SURF_SOURCES benchmark/*.cpp)
  add_executable(benchmark_surf ${BENCHMARK_SURF_SOURCES})
  set_target_properties(benchmark_surf PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
  target_compile_options(benchmark_surf PRIVATE ${WARNINGS_CXX_FLAGS})
  target_link_libraries(benchmark_surf
    surf
    benchmark::benchmark)
endif()

if(BUILD_EXTRA)
  file(GLOB EXTRA_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    extra/*.cpp)
  foreach(SRC ${EXTRA_SOURCES})
    get_filename_component(SRC_BASENAME ${SRC} NAME_WE)
    add_executable(${SRC_BASENAME} ${SRC})
    set_target_properties(${SRC_BASENAME} PROPERTIES
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSIONS OFF)
    target_compile_options(${SRC_BASENAME} PRIVATE ${WARNINGS_CXX_FLAGS})
    target_link_libraries(${SRC_BASENAME} surf)
    install(TARGETS ${SRC_BASENAME}
      RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
  endforeach(SRC)
endif()

install(DIRECTORY include/. DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

include(ExportAndInstallLibrary)

# EOF #
