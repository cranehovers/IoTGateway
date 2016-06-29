
# This file defines toolkit direcotry building logic

AUX_SOURCE_DIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}"  source_files)
SET(ALL_SOURCES_LIST ${ALL_SOURCES_LIST} ${source_files} CACHE INTERNAL "all source files")

