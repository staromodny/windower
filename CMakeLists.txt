cmake_minimum_required(VERSION 3.25)

project(Windower
    VERSION 1.0
    DESCRIPTION "Lightweight Win32 window wrapper"
    LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "Windower only supports Windows.")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

file(GLOB_RECURSE WINDOWER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE WINDOWER_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

include(GNUInstallDirs)

add_library(Windower
        ${WINDOWER_SOURCES}
        ${WINDOWER_HEADERS}
)

target_link_libraries(Windower PRIVATE Dwmapi)

add_library(Windower::Windower ALIAS Windower)

target_include_directories(Windower
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS Windower
        EXPORT WindowerTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT WindowerTargets
        FILE WindowerTargets.cmake
        NAMESPACE Windower::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Windower
)