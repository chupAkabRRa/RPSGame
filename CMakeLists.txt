cmake_minimum_required(VERSION 3.7)

project(RPSGame)

set(CMAKE_CXX_STANDARD 14)

# Check the compiler requirements
if (WIN32)
    if ((NOT ${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64") AND (NOT ${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015"))
        message(FATAL_ERROR "Visual Studio 14 2015 need to be installed!")
    endif()
endif(WIN32)

# 1) Setting up base includes
set(BASE_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/base")
set(GAME_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/game/include")

# 2) Setting up SDL libraries depending on platform
if (WIN32)
    if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/win/sdl/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/win/sdl/x64/SDL2main.lib")
    else()
        set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/win/sdl/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/lib/win/sdl/x86/SDL2main.lib")
    endif()
endif (WIN32)

# 3) Preparing everything for project generation
include_directories(${GAME_INCLUDE_DIRS} ${BASE_INCLUDE_DIRS})
add_executable(RPSGame RPSGame.cpp
                       ./base/config/RPSConfig.cpp
                       ./base/logger/easylogging++.cpp
                       ./game/src/RPSEngine.cpp
                       ./game/src/RPSCore.cpp)
target_link_libraries(RPSGame ${SDL2_LIBRARIES})
if (WIN32)
    if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
        install(TARGETS RPSGame DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin/win/x64)
    else()
        install(TARGETS RPSGame DESTINATION ${CMAKE_CURRENT_LIST_DIR}/bin/win/x86)
    endif()
endif(WIN32)

# 4) Don't forget to copy important files
if (WIN32)
    if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
        set(SDL_DLL_ORIGIN_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/win/sdl/x64/SDL2.dll)
        set(FILES_DESTINATION_PATH ${CMAKE_CURRENT_LIST_DIR}/bin/win/x64)
    else()
        set(SDL_DLL_ORIGIN_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/win/sdl/x86/SDL2.dll)
        set(FILES_DESTINATION_PATH ${CMAKE_CURRENT_LIST_DIR}/bin/win/x86)
    endif()
endif(WIN32)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/RPSLog.conf
    DESTINATION ${FILES_DESTINATION_PATH})
file(COPY ${SDL_DLL_ORIGIN_PATH}
    DESTINATION ${FILES_DESTINATION_PATH})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/RPSGame.conf
    DESTINATION ${FILES_DESTINATION_PATH})