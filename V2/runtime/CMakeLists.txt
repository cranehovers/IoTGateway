
# This file defines runtime direcotry building logic

AUX_SOURCE_DIRECTORY("${PROJECT_SOURCE_DIR}/runtime" runtime_source_files)
SET(G_RUNTIME_SOURCES ${runtime_source_files} CACHE INTERNAL "runtime soruces")
SET(ALL_SOURCES_LIST ${ALL_SOURCES_LIST} ${G_RUNTIME_SOURCES} CACHE INTERNAL "all source files")


