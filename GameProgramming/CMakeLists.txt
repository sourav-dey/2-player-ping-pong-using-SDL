# CMakeList.txt : CMake project for GameProgramming, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.13)

# Add source to this project's executable.
#set(HEADERS "Game.h" "GameProgramming.h")
#set(SOURCE "Game.cpp" "GameProgramming.cpp")

add_executable (GameProgramming Game.cpp Game.h)

# TODO: Add tests and install targets if needed.
target_link_libraries(GameProgramming ${SDL2_LIBRARIES})

add_custom_command(TARGET GameProgramming POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/3rdPartyLibs/SDL2-2.0.10/lib/x64/SDL2.dll"             
    $<TARGET_FILE_DIR:GameProgramming>)