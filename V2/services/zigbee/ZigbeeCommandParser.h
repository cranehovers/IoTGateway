/*
* This file defines Zigbee serial port parser
*/

#ifndef __ZIGBEE_SERIALPORT_PARSER_H__
#define __ZIGBEE_SERIALPORT_PARSER_H__

#include <list>

namespace Services{
namespace Zigbee{

class ZigbeeCommand;

class DataFrame
{
public:

    DataFrame(){
        token_len = 0;
        cmd0 = 0;
        cmd1 = 0;
        fcs = 0;
        data = 0;
        state = 0;
        copied_len = 0;
    };

    unsigned char token_len;
    unsigned char cmd0;
    unsigned char cmd1;
    unsigned char fcs;
    unsigned char  *data;
    unsigned char state;
    unsigned char copied_len;

    void copy(unsigned char *buf, unsigned char &len);
};

class ZigbeeCommandParser
{
public:
    ZigbeeCommandParser();
    ~ZigbeeCommandParser();
    
    int add_data(unsigned char *data, unsigned int len);
    ZigbeeCommand *get_frame();

    void clear();
    
private:

    std::list<DataFrame*> frame_list;
    DataFrame *current_frame;
    
};


}
}


#endif //__ZIGBEE_SERIALPORT_PARSER_H__
