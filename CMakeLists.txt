cmake_minimum_required(VERSION 3.6)
project(GameDesign)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.c
        GameStateManager.c
        GameStateManager.h
        System.c
        System.h
        Level1.c
        Level1.h
        Level2.c
        Level2.h
        GameStateList.h
        Input.c
        Input.h
        Level1_Counter.txt
        Level2_Counter.txt
        Level2_Lives.txt
        README.md)

add_executable(GameDesign ${SOURCE_FILES} main.c  GameStateManager.c GameStateManager.h System.c System.h Level1.c Level1.h Level2.c Level2.h)