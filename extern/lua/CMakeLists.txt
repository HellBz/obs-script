cmake_minimum_required (VERSION 2.6)
project (lua C)

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
    message(STATUS "Adding Lua support for obs-script")
    set(HAS_LUA 1)
else()
    message(STATUS "Lua src not found, will not include Lua for obs-script")
    set(HAS_LUA 0)
endif()

set(HAS_LUA ${HAS_LUA} PARENT_SCOPE)

if (HAS_LUA)
    if(WIN32)
            add_definitions( -D_CRT_SECURE_NO_WARNINGS )
    endif()

    add_library(lua STATIC
        src/lapi.c
        src/lcode.c
        src/lctype.c
        src/ldebug.c
        src/ldo.c
        src/ldump.c
        src/lfunc.c
        src/lgc.c
        src/llex.c
        src/lmem.c
        src/lobject.c
        src/lopcodes.c
        src/lparser.c
        src/lstate.c
        src/lstring.c
        src/ltable.c
        src/ltm.c
        src/lundump.c
        src/lvm.c
        src/lzio.c
        src/lauxlib.c
        src/lbaselib.c
        src/lbitlib.c
        src/lcorolib.c
        src/ldblib.c
        src/liolib.c
        src/lmathlib.c
        src/loslib.c
        src/lstrlib.c
        src/ltablib.c
        src/loadlib.c
        src/linit.c
        src/lutf8lib.c)

    target_include_directories(lua PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

endif()