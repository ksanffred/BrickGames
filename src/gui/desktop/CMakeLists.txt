cmake_minimum_required(VERSION 3.10)

project(BrickGame VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

option(BUILD_SNAKE "Build Snake Game" ON)
option(BUILD_TETRIS "Build Tetris Game" ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

set(FRONTEND_SOURCES
    controller.cc
)
set(FRONTEND_HEADERS
    controller.h
)

set(SNAKE_BACKEND_SOURCES
    ../../brick_game/snake/snake_back.cc
)
set(SNAKE_BACKEND_HEADERS
    ../../brick_game/snake/snake_back.h
)

set(TETRIS_BACKEND_SOURCES
    ../../brick_game/tetris/back_tetris.c
    ../../brick_game/tetris/fsm_tetris.c
)
set(TETRIS_BACKEND_HEADERS
    ../../brick_game/tetris/back_fsm_tetris.h
)

function(add_brickgame_executable BUILD_GAME_NAME BACKEND_SOURCES BACKEND_HEADERS)

    add_executable(${BUILD_GAME_NAME}
        ${FRONTEND_SOURCES}
        ${BACKEND_SOURCES}
        ${FRONTEND_HEADERS}
        ${BACKEND_HEADERS}
        ${RESOURCES}
    )

    target_link_libraries(${BUILD_GAME_NAME} PRIVATE
        Qt6::Quick
    )

    target_include_directories(${BUILD_GAME_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../brick_game/snake
        ${CMAKE_CURRENT_SOURCE_DIR}/../../brick_game/tetris
    )   

    if(${BUILD_GAME_NAME} STREQUAL "SnakeGame")
        target_compile_definitions(${BUILD_GAME_NAME} PRIVATE "BUILD_SNAKE")
    elseif(${BUILD_GAME_NAME} STREQUAL "TetrisGame")
        target_compile_definitions(${BUILD_GAME_NAME} PRIVATE "BUILD_TETRIS")
    endif()

    qt_add_qml_module(${BUILD_GAME_NAME}
        URI "front_${BUILD_GAME_NAME}"
        VERSION 1.0
        QML_FILES front.qml
    )
endfunction()

if (BUILD_SNAKE)
    add_brickgame_executable(SnakeGame
        ${SNAKE_BACKEND_SOURCES} 
        ${SNAKE_BACKEND_HEADERS}
    )
endif()

if (BUILD_TETRIS)
    add_brickgame_executable(TetrisGame
        ${TETRIS_BACKEND_SOURCES}
        ${TETRIS_BACKEND_HEADERS}
    )
endif()
