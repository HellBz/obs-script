cmake_minimum_required(VERSION 2.8.12)

set(HAS_LUA 0)
set(HAS_PYTHON 0)

add_subdirectory(lua)
#add_subdirectory(python)

set(HAS_LUA ${HAS_LUA} PARENT_SCOPE)
set(HAS_PYTHON ${HAS_PYTHON} PARENT_SCOPE)