cmake_minimum_required(VERSION 3.21)

PROJECT(dmlog)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

#SET(CMAKE_CXX_CPPCHECK "cppcheck")
#LIST(APPEND CMAKE_CXX_CPPCHECK "--enable=all")

ModuleImport("dmformat" "thirdparty/dmformat")

if(PROJECT_IS_TOP_LEVEL)
    ExeImport("test" "dmformat")
endif()
