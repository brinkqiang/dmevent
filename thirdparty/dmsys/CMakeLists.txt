cmake_minimum_required(VERSION 3.2)

PROJECT(dmsys)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(cmake/ModuleImport.cmake)
INCLUDE(cmake/ModuleCompileOptions.cmake)
ModuleSetCompileOptions()

SET(DMSYS_VERSION_MAJOR "1")
SET(DMSYS_VERSION_MINOR "0")
SET(DMSYS_VERSION_PATCH "1")
SET(DMSYS_VERSION "${DMSYS_VERSION_MAJOR}.${DMSYS_VERSION_MINOR}.${DMSYS_VERSION_PATCH}")

MESSAGE(STATUS "VERSION: ${DMSYS_VERSION}")

ADD_DEFINITIONS(-DUSE_DMSYS)

ModuleImport("dmtimer" "thirdparty/dmtimer")
ModuleImport("dmformat" "thirdparty/dmformat")
ModuleImport("dmtest" "thirdparty/dmtest")
ModuleImport("dmxlsx" "thirdparty/dmxlsx")
ModuleImport("dmprotobuf" "thirdparty/dmprotobuf")

LibImportDepends("dmsys" "src" "libprotobuf-lite;libprotoc;dmtimer;dmxlsx;dmxlsximpl")

ExeImport("test" "libprotobuf-lite;libprotoc;dmsys;dmtimer;dmtest;dmxlsx;dmxlsximpl")

#ADD_CUSTOM_COMMAND(
#TARGET dmsystest
#PRE_BUILD
#COMMAND ${CMAKE_COMMAND} -E copy_if_different
#${CMAKE_CURRENT_SOURCE_DIR}/config/test.csv
#$<TARGET_FILE_DIR:dmsystest>
#)

#INSTALL(DIRECTORY include/ DESTINATION include)
#INSTALL(TARGETS dmsys
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib)

#SET(DMSYS_LIBS "dmsys")
#MARK_AS_ADVANCED(${DMSYS_LIBS})