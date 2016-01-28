/*
* This file defines serialport command
*/

#ifndef __ZIGBEE_SERIALPORT_COMMAND_H__
#define __ZIGBEE_SERIALPORT_COMMAND_H__

#include "Config.h"


class ZigbeeSerialportCommand
{
public:

    ZigbeeSerialportCommand();
    ~ZigbeeSerialportCommand();

    static ZigbeeSerialportCommand *create_from_buffer(unsigned char *buf, unsigned short len);
    static ZigbeeSerialportCommand *create_EP_SIMPLE_DESC_cmd(unsigned char dst_short_addr[2],
                                                              unsigned char interest_short_addr[2],
                                                              unsigned char ep);                                                                    
    static ZigbeeSerialportCommand *create_ACTIVE_EP_cmd(unsigned char dst_short_addr[2], 
                                                         unsigned char interest_short_addr[2]);
    static ZigbeeSerialportCommand *create_IEEE_ADDR_cmd(unsigned char short_addr[2],
                                                          unsigned char req_type,
                                                          unsigned char start_indx);
                                                          
    void free();
    unsigned char *base();
    unsigned int size();
    unsigned int command_size;
    unsigned char *command;
};

#endif //__ZIGBEE_SERIALPORT_COMMAND_H__