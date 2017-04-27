cmake_minimum_required(VERSION 3.6)
project(GameDesign)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        lib/3DPipelineTools.h
        lib/AECollision.h
        lib/AEEngine.h
        lib/AEExport.h
        lib/AEFrameRateController.h
        lib/AEGameStateMgr.h
        lib/AEGraphics.h
        lib/AEInput.h
        lib/AELineSegment2.h
        lib/AEMath.h
        lib/AEMtx33.h
        lib/AESystem.h
        lib/AETypes.h
        lib/AEUtil.h
        lib/AEVec2.h
        lib/Alpha_Engine.lib
        GameStateList.h
        GameStateManager.c
        GameStateManager.h
        Input.c
        Input.h
        Level1.c
        Level1.h
        Level2.c
        Level2.h
        main.c
        System.c
        System.h)
add_executable(GameDesign ${SOURCE_FILES})