###############################################################################
##
## Copyright (C) 2023 Yura Beznos at You-ra.info
##
## This file is part of the Screen recorder application project.
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write an email to the license@you-ra.info
###############################################################################

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER "/usr/bin/gcc")

project(info.you_ra.screen_recorder C CXX)
cmake_minimum_required(VERSION 3.5)


find_package(Qt5 COMPONENTS Core Network Qml Gui Quick LinguistTools REQUIRED)
set(AURORAOS_SHARED_LIBRARY_DIR /usr/share/${PROJECT_NAME}/lib)
set(short_name screen_recorder)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fPIC -pie -rdynamic -Wall -Wl,-rpath=/usr/share/${PROJECT_NAME}/lib")

find_library(AVCODEC_LIBRARY avcodec)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARY swscale)

include(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_search_module(AURORAAPP auroraapp REQUIRED IMPORTED_TARGET)
pkg_search_module(LIBVNCCLIENT libvncclient REQUIRED IMPORTED_TARGET)

include(QtTranslationWithID.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# DEBUG
set(CMAKE_DEBUG_TARGET_PROPERTIES
  INCLUDE_DIRECTORIES
  COMPILE_DEFINITIONS
  POSITION_INDEPENDENT_CODE
  CONTAINER_SIZE_REQUIRED
  LIB_VERSION
)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src/
)

set(screenrecorder_SRCS
    src/main.cpp
    src/interfacer.cpp
    src/recordingsmodel.cpp
    src/settings.cpp
    src/recordthread.cpp
    src/vnc2mpg.c
)
file(GLOB_RECURSE screenrecorder_QML_SRCS qml/*.qml)

set(screenrecorder_TS_SRCS
    ${screenrecorder_SRCS}
    ${screenrecorder_QML_SRCS}
)

create_translation(engen_qm_file ${CMAKE_SOURCE_DIR}/translations/screen_recorder.ts ${CMAKE_SOURCE_DIR}/translations/screen_recorder_ru.ts ${screenrecorder_TS_SRCS})

FILE(GLOB TsFiles "translations/*.ts")
qt5_add_translation(QmFiles ${TsFiles})

add_executable(${PROJECT_NAME}
    ${screenrecorder_SRCS}
    ${engen_qm_file}
    ${QmFiles}
    ${FOLEYS_ARG_BUILD_TARGET})

target_link_libraries(${PROJECT_NAME}
    PkgConfig::AURORAAPP
    PkgConfig::LIBVNCCLIENT
    Qt5::Quick
    ${AVCODEC_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${AVFORMAT_LIBRARY}
    ${SWSCALE_LIBRARY}
    -lpthread)

set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)


install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
install(DIRECTORY qml
    DESTINATION share/info.you_ra.screen_recorder
)

foreach(_file IN LISTS QmFiles)
    get_filename_component(_filename "${_file}" NAME)
    string(REPLACE "${short_name}" "${PROJECT_NAME}" _full_filename "${_filename}")
    install(FILES ${_file} DESTINATION share/info.you_ra.screen_recorder/translations)# RENAME ${_full_filename})
endforeach()
#install(FILES ${engen_qm_file} DESTINATION share/info.you_ra.screen_recorder/translations RENAME ${_full_filename})

install(FILES info.you_ra.screen_recorder.desktop
    DESTINATION share/applications
)
install(FILES icons/86x86/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/86x86/apps
)
install(FILES icons/108x108/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/108x108/apps
)
install(FILES icons/128x128/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/128x128/apps
)
install(FILES icons/172x172/info.you_ra.screen_recorder.png
    DESTINATION share/icons/hicolor/172x172/apps
)

# Get the other files reachable from the project tree in Qt Creator
FILE(GLOB TsFiles "translations/*.ts")
add_custom_target(distfiles
    SOURCES
        info.you_ra.screen_recorder.desktop
        qml/screen_recorder.qml
        qml/cover/DefaultCoverPage.qml
        qml/pages/MainPage.qml
        qml/pages/AboutPage.qml
        qml/pages/Settings.qml
        qml/pages/RenameDialog.qml
        qml/pages/ViewVideoPage.qml
        qml/components/RecordingDelegate.qml
        qml/components/MediaTitleOverlay.qml
        rpm/info.you_ra.screen_recorder.spec
        rpm/info.you_ra.screen_recorder.changes.in
        rpm/info.you_ra.screen_recorder.changes.run.in
        ${TsFiles})

file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/info.you_ra.screen_recorder:bin
")

