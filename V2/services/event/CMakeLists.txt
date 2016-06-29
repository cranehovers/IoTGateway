
# This file defines toolkit direcotry building logic

AUX_SOURCE_DIRECTORY("${PROJECT_SOURCE_DIR}/services/event" event_source_files)
SET(G_EVENT_SOURCES ${event_source_files} CACHE INTERNAL "service soruces")
SET(ALL_SOURCES_LIST ${ALL_SOURCES_LIST} ${G_EVENT_SOURCES} CACHE INTERNAL "all source files")


