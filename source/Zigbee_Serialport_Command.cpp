/*
* This file defins zigbee serial port command
*/

#include "Zigbee_Serialport_Command.h"

#define MT_UART_SOF                     0xFE


#define MT_ZDO_IEEE_ADDR_REQ0                 0x25
#define MT_ZDO_IEEE_ADDR_REQ1                 0x01

#define MT_ZDO_ACTIVE_EP_REQ0                 0x25
#define MT_ZDO_ACTIVE_EP_REQ1                 0x05

#define MT_ZDO_EP_DESC_REQ0                 0x25
#define MT_ZDO_EP_DESC_REQ1                 0x04

static unsigned char calc_xor( unsigned char *data, unsigned char len )
{
  unsigned char x;
  unsigned char xorResult;

  xorResult = 0;

  for ( x = 0; x < len; x++, data++ )
    xorResult = xorResult ^ *data;

  return ( xorResult );
}


ZigbeeSerialportCommand::ZigbeeSerialportCommand()
{
    command_size = 0;
    command = 0;

}

ZigbeeSerialportCommand::~ZigbeeSerialportCommand()
{
    free();
}

void ZigbeeSerialportCommand::free()
{
    if (command != 0)
    {
        delete command;
        command = 0;
    }
}

unsigned char *ZigbeeSerialportCommand::base()
{
    if (command)
    {
        return command;
    }
    
    return 0; 
}

unsigned int ZigbeeSerialportCommand::size()
{
    if (command_size)
    {
        return command_size;
    }
    
    return 0;     
}

ZigbeeSerialportCommand *ZigbeeSerialportCommand::create_from_buffer(unsigned char *buf, unsigned short len)
{
    ZigbeeSerialportCommand *cmd = new ZigbeeSerialportCommand();
    
    cmd->command_size = len;
    cmd->command = new unsigned char[cmd->command_size];

    ACE_OS::memcpy(cmd->command, buf, len);

    return cmd;
}

ZigbeeSerialportCommand *ZigbeeSerialportCommand::create_IEEE_ADDR_cmd(unsigned char short_addr[2],
                                                      unsigned char req_type,
                                                      unsigned char start_indx)
{
    ZigbeeSerialportCommand *cmd = new ZigbeeSerialportCommand();

    cmd->command_size = 9;
    cmd->command = new unsigned char[cmd->command_size];

    cmd->command[0] = MT_UART_SOF;
    cmd->command[1] = 4;
    cmd->command[2] = MT_ZDO_IEEE_ADDR_REQ0;
    cmd->command[3] = MT_ZDO_IEEE_ADDR_REQ1;
    cmd->command[4] = short_addr[0];
    cmd->command[5] = short_addr[1];
    cmd->command[6] = req_type;
    cmd->command[7] = start_indx;
    cmd->command[8] = calc_xor(&(cmd->command[1]), 7);

    return cmd;
    
}

ZigbeeSerialportCommand *ZigbeeSerialportCommand::create_ACTIVE_EP_cmd(unsigned char dst_short_addr[2], 
                                                                       unsigned char interest_short_addr[2])
{
    ZigbeeSerialportCommand *cmd = new ZigbeeSerialportCommand();

    cmd->command_size = 9;
    cmd->command = new unsigned char[cmd->command_size];

    cmd->command[0] = MT_UART_SOF;
    cmd->command[1] = 4;
    cmd->command[2] = MT_ZDO_ACTIVE_EP_REQ1;
    cmd->command[3] = MT_ZDO_ACTIVE_EP_REQ1;
    cmd->command[4] = dst_short_addr[0];
    cmd->command[5] = dst_short_addr[1];
    cmd->command[6] = interest_short_addr[0];
    cmd->command[7] = interest_short_addr[1];
    cmd->command[8] = calc_xor(&(cmd->command[1]), 7);

    return cmd;

}
                                                     
ZigbeeSerialportCommand *ZigbeeSerialportCommand::create_EP_SIMPLE_DESC_cmd(unsigned char dst_short_addr[2], 
                                                                    unsigned char interest_short_addr[2],
                                                                    unsigned char ep)
{
 ZigbeeSerialportCommand *cmd = new ZigbeeSerialportCommand();

 cmd->command_size = 10;
 cmd->command = new unsigned char[cmd->command_size];

 cmd->command[0] = MT_UART_SOF;
 cmd->command[1] = 0x05;
 cmd->command[2] = MT_ZDO_EP_DESC_REQ0;
 cmd->command[3] = MT_ZDO_EP_DESC_REQ1;
 cmd->command[4] = dst_short_addr[0];
 cmd->command[5] = dst_short_addr[1];
 cmd->command[6] = interest_short_addr[0];
 cmd->command[7] = interest_short_addr[1];
 cmd->command[8] = ep;
 cmd->command[9] = calc_xor(&(cmd->command[1]), 8);

 return cmd;

}
                                                                                                          
ZigbeeSerialportCommand *ZigbeeSerialportCommand::create_AF_DATA_REQ_cmd(unsigned char dst_short_addr[2],
                                                     unsigned char dst_ep,
                                                     unsigned char src_ep,
                                                     unsigned char cluster_id[2],
                                                     unsigned char data_len,
                                                     unsigned char *data_buf
                                                     )
{
    unsigned char tmp_buf[0xff];
    unsigned char i = 0;

    tmp_buf[i++] = MT_UART_SOF;
    tmp_buf[i++] = 0; // must set the length later again
    tmp_buf[i++] = 0x24;
    tmp_buf[i++] = 0x01;
    tmp_buf[i++] = dst_short_addr[0];
    tmp_buf[i++] = dst_short_addr[1];
    tmp_buf[i++] = dst_ep;
    tmp_buf[i++] = src_ep;
    tmp_buf[i++] = cluster_id[0];
    tmp_buf[i++] = cluster_id[1];
    tmp_buf[i++] = 0;
    tmp_buf[i++] = 0;
    tmp_buf[i++] = 0;
    tmp_buf[i++] = data_len;

    ACE_OS::memcpy(&tmp_buf[i], data_buf, data_len);

    i += data_len;

    tmp_buf[1] = (i-4);
    tmp_buf[i] = calc_xor(&(tmp_buf[1]), (i-1));

    ZigbeeSerialportCommand *cmd = new ZigbeeSerialportCommand();
    
    cmd->command_size = i+1;
    cmd->command = new unsigned char[cmd->command_size];
    
    ACE_OS::memcpy(cmd->command, tmp_buf, cmd->command_size);

    return cmd;


}

