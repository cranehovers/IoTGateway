/*
* This file defines serialport command
*/

#ifndef __ZIGBEE_SERIALPORT_COMMAND_H__
#define __ZIGBEE_SERIALPORT_COMMAND_H__

namespace Services{
namespace Zigbee{

class ZigbeeCommand
{
public:

    ZigbeeCommand();
    ~ZigbeeCommand();

    static ZigbeeCommand *create_from_buffer(unsigned char *buf, unsigned short len);
    static ZigbeeCommand *create_EP_SIMPLE_DESC_cmd(unsigned char dst_short_addr[2],
                                                              unsigned char interest_short_addr[2],
                                                              unsigned char ep);                                                                    
    static ZigbeeCommand *create_ACTIVE_EP_cmd(unsigned char dst_short_addr[2], 
                                                         unsigned char interest_short_addr[2]);
    static ZigbeeCommand *create_IEEE_ADDR_cmd(unsigned char short_addr[2],
                                                          unsigned char req_type,
                                                          unsigned char start_indx);
                                                          
    static ZigbeeCommand *create_AF_DATA_REQ_cmd(unsigned char dst_short_addr[2],
                                                           unsigned char dst_ep,
                                                           unsigned char src_ep,
                                                           unsigned char cluster_id[2],
                                                           unsigned char data_len,
                                                           unsigned char *data_buf
                                                           );
                                                          
    void free();
    unsigned char *base();
    unsigned int size();
    unsigned int command_size;
    unsigned char *command;
};

}
}

#endif //__ZIGBEE_SERIALPORT_COMMAND_H__
