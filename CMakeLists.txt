cmake_minimum_required(VERSION 3.8)

PROJECT(dmevent)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

SET(DMEVENT_VERSION_MAJOR "1")
SET(DMEVENT_VERSION_MINOR "0")
SET(DMEVENT_VERSION_PATCH "1")
SET(DMEVENT_VERSION "${DMEVENT_VERSION_MAJOR}.${DMEVENT_VERSION_MINOR}.${DMEVENT_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${DMEVENT_VERSION}")

OPTION(USE_DMEVENT "use dmevent" OFF)

ADD_DEFINITIONS(-DASIO_STANDALONE)

ModuleInclude("asio" "thirdparty/asio")
ModuleImport("dmlog" "thirdparty/dmlog")

LibImport("dmevent" "src/dmevent")

if(PROJECT_IS_TOP_LEVEL)
    ExeImport("example" "dmevent")
endif()
