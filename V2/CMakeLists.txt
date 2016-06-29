# This file defines Gateway project building logic

PROJECT(gwsp_app)
cmake_minimum_required(VERSION 2.8)

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR} source_files)
SET(ALL_SOURCES_LIST ${source_files} CACHE INTERNAL "all source files")

#add into all sub directory
ADD_SUBDIRECTORY(toolkit)
ADD_SUBDIRECTORY(services)
ADD_SUBDIRECTORY(runtime)
ADD_SUBDIRECTORY(server)

