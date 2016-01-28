/*
* This file defines Zigbee serial port
*/

#ifndef __ZIGBEE_SERIAL_PORT_H__
#define __ZIGBEE_SERIAL_PORT_H__

#include "Config.h"
#include <deque>
#include <vector>

class ACE_TTY_IO;
class CfgService;
class NetService;
class ZigbeeSerialportCommand;
class ZigbeeSerialportParser;
class ZigbeeSerialportService;
class ZigbeeNotify;
class ZigbeeRequest;

class SerialportTask :public ACE_Task_Base
{
public:
    SerialportTask();
    ~SerialportTask();

    void set_owner(ZigbeeSerialportService *o);
    
    virtual int svc (void);
    void clear();

    ZigbeeSerialportService *owner_;
    unsigned short thread_exit_flag;    
    ACE_Message_Queue<ACE_MT_SYNCH> message_queue;
};

class ZigbeeSerialportService :public ACE_Event_Handler
{
public:

    friend class SerialportTask;
    
    typedef enum SerialPortState
    {
        Idle,
        Opened,
        Sended,
    }SerialPortState;

    static ZigbeeSerialportService *instance();    
    static ZigbeeSerialportService *instance_;
    
    ZigbeeSerialportService(CfgService *conf, NetService *net);
    ~ZigbeeSerialportService();
    
    int Init();
    int Close();
    
    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);
    
    virtual ACE_HANDLE get_handle (void) const;

    int send(ZigbeeRequest *req);

    void register_notify_node(ZigbeeNotify *n);

private:

    int open_serial_port();
    void exit_and_wait_thread();
    void connect_to_serialport();
    void connecting_serial_port();
    void notify_serialport_connected();
    void notify(unsigned short n);
    void send_req(unsigned char *buf, unsigned int len);
    void get_response();

    ACE_TTY_IO *io_svc_;
    CfgService *conf_;
    NetService *net_;
    SerialPortState state_;
    ZigbeeSerialportParser *parser_;
    SerialportTask *task_;
    std::vector<ZigbeeNotify*> notify_list_;
    
};


#endif //__ZIGBEE_SERIAL_PORT_H__
