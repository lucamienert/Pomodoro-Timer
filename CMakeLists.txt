cmake_minimum_required(VERSION 3.16)
project(Gtk4Pomodoro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

add_executable(pomodoro
    main.cpp
    PomodoroWindow.cpp
)

target_include_directories(pomodoro PRIVATE ${GTKMM_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_directories(pomodoro PRIVATE ${GTKMM_LIBRARY_DIRS})
target_link_libraries(pomodoro PRIVATE ${GTKMM_LIBRARIES})
target_compile_options(pomodoro PRIVATE ${GTKMM_CFLAGS_OTHER})
