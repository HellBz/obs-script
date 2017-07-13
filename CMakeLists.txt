cmake_minimum_required(VERSION 2.8.12)

project(obs-script C CXX)

add_subdirectory(extern)

if (HAS_LUA)
    add_definitions(-DHAS_LUA)
endif()

if (HAS_PYTHON)
    add_definitions(-DHAS_PYTHON)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

macro( source_group_tree )
    file( GLOB all_files ${ARGV1}/* )
    set( files "" )
    foreach( each ${all_files} )
        if( NOT IS_DIRECTORY ${each} )
            get_filename_component( file_name ${each} NAME )
            list( APPEND files "${ARGV1}/${file_name}" )
        endif()
    endforeach()
    list( LENGTH files files_len )
    if( ${files_len} GREATER 0 )
        string( REPLACE "/" "\\\\" group_name ${ARGV0} )
        source_group("${group_name}" FILES ${files})
    endif()

    file( GLOB directories ${ARGV1}/*/ )
    foreach( each ${directories} )
        if( IS_DIRECTORY ${each} )
            get_filename_component( file_name ${each} NAME )
            source_group_tree("${ARGV0}/${file_name}" "${ARGV1}/${file_name}")
        endif()
    endforeach()
endmacro()

set(INCLUDES
    src/types.h
    src/ScriptModule.h
    src/ScriptMan.h
    src/RefCounted.h
    src/Interface/Context.h
    src/Interface/Script.h
    src/Interface/Function.h
    src/Interface/Class.h
    src/Lua/Context.h
    src/Lua/Script.h
    src/Lua/Function.h
    src/Lua/Class.h
    src/Lua/Utils.h
    src/Reflection/ClassWalker.h
    src/Reflection/Function.h
    src/Reflection/ClassRegistry.h
    src/Exposed/Text.h
    src/Exposed/Source.h
    src/Utils/TypeNames.h
    src/Utils/ReadWriteMutex.h)

set(SOURCES
    src/obs-script.cpp
    src/ScriptModule.cpp
    src/ScriptMan.cpp
    src/Lua/Context.cpp
    src/Lua/Script.cpp
    src/Lua/Function.cpp
    src/Lua/Class.cpp
    src/Reflection/ClassRegistry.cpp
    src/Exposed/Text.cpp
    src/Exposed/Source.cpp
    src/Utils/TypeNames.cpp
    src/Utils/ReadWriteMutex.cpp)

source_group_tree(Source src)

add_library(obs-script MODULE ${INCLUDES} ${SOURCES})
add_dependencies(obs-script lua)
target_link_libraries(obs-script
    libobs
    lua)

install_obs_plugin_with_data(obs-script data)