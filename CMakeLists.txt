cmake_minimum_required(VERSION 3.0)

project(spirograph)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 REQUIRED audio graphics window system)
find_package(TGUI 0.9.0 REQUIRED)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else ()
    message(ERROR_FATAL "Can not find SFML")
endif ()

set(PACKAGES "${PROJECT_NAME}")

add_executable(cycloid cycloid.cpp)

target_link_libraries(cycloid
        ${SFML_LIBRARIES}
        ${SFML_DEPENDENCIES}
        sfml-graphics
        sfml-window
        sfml-system
        tgui
        m)

add_executable(epitrochoid epitrochoid.cpp)

target_link_libraries(epitrochoid
        ${SFML_LIBRARIES}
        ${SFML_DEPENDENCIES}
        sfml-graphics
        sfml-window
        sfml-system
        tgui
        m)