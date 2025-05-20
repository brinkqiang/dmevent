cmake_minimum_required(VERSION 3.21)

PROJECT(dmevent)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

ADD_DEFINITIONS(-DASIO_STANDALONE)

ModuleInclude("asio" "thirdparty/asio")
ModuleImport("dmlog" "thirdparty/dmlog")

LibImport("dmevent" "src/dmevent")

if(PROJECT_IS_TOP_LEVEL)
    ExeImport("example" "dmevent")
endif()
