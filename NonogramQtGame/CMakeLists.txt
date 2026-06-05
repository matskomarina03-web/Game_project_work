cmake_minimum_required(VERSION 3.16)

project(NonogramGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Test)
if (Qt6_FOUND)
    qt_add_executable(NonogramGame
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        minigameswidget.cpp
        minigameswidget.h

        mazeadventurewidget.h
        progressservice.cpp
        progressservice.h
        round.cpp
        round.h
        mazeadventurewidget.cpp
        gamevalidator.h gamevalidator.cpp

    )
    target_link_libraries(NonogramGame PRIVATE Qt6::Widgets Qt6::Core)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    add_executable(NonogramGame
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        minigameswidget.cpp
        minigameswidget.h
        mazeadventurewidget.cpp
        mazeadventurewidget.h
        progressservice.cpp
        progressservice.h
        round.cpp
        round.h
    )
    target_link_libraries(NonogramGame PRIVATE Qt5::Widgets)
endif()

add_custom_command(TARGET NonogramGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/rounds.json
        $<TARGET_FILE_DIR:NonogramGame>/rounds.json
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/maze_levels.json
        $<TARGET_FILE_DIR:NonogramGame>/maze_levels.json
)


add_executable(GameValidatorTests
    tst_gamevalidator.cpp
    gamevalidator.cpp
)

target_link_libraries(GameValidatorTests PRIVATE Qt6::Core Qt6::Test)
target_include_directories(GameValidatorTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
