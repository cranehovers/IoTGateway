# This file defines server building logic

set(GWSP gwsp_server)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/3rd/ace/include
                    ${PROJECT_SOURCE_DIR}/3rd/libcoap/include
                    )


AUX_SOURCE_DIRECTORY("${PROJECT_SOURCE_DIR}/server" server_sources)

add_definitions(-DWITH_POSIX)


ADD_EXECUTABLE(${GWSP} 
			   ${server_sources} 
               ${ALL_SOURCES_LIST} 
              )

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/3rd/ace/lib
                 ${PROJECT_SOURCE_DIR}/3rd/libcoap/lib
                )   

FIND_LIBRARY(ACE
            NAMES libACE.so
            PATHS ${PROJECT_SOURCE_DIR}/3rd/ace/lib
            )

FIND_LIBRARY(coap
            NAMES libcoap.a
            PATHS ${PROJECT_SOURCE_DIR}/3rd/libcoap/lib
            )


message(${ACE})

TARGET_LINK_LIBRARIES(${GWSP} 
                      ${ACE}
                      ${coap}
                    )
