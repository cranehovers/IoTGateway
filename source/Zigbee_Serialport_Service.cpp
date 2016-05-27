/*
* This file defins zigbee serial port
*/

#include "Zigbee_Serialport_Service.h"
#include "CfgService.h"
#include "NET_Service.h"
#include "Zigbee_Serialport_Command.h"
#include "Zigbee_Serialport_Parser.h"
#include "Zigbee_Notify.h"
#include "Zigbee_Request.h"


#define ZIGBEE_SERIAL_PORT_CMD_EXIT_THREAD 0x80
#define ZIGBEE_SERIAL_PORT_CMD_CONNECT_PORT 0x81
#define ZIGBEE_SERIAL_PORT_CMD_SEND_REQ 0x82
#define ZIGBEE_SERIAL_PORT_CMD_GET_RES 0x83

static void dump_(unsigned char *buf, unsigned int len)
{
    ACE_DEBUG((LM_DEBUG, ">>>> "));
    for (int i=0; i < len; i++)
        ACE_DEBUG((LM_DEBUG,"%0x ",buf[i]));

    ACE_DEBUG((LM_DEBUG, "\n"));

}

SerialportTask::SerialportTask()
{
    thread_exit_flag = 0;
}

SerialportTask::~SerialportTask()
{
   clear();
}

void SerialportTask::clear()
{
    while (!message_queue.is_empty())
    {
        ACE_Message_Block *b = 0;
        message_queue.dequeue(b);

        b->release();
    }
}

void SerialportTask::set_owner(ZigbeeSerialportService *o)
{
    owner_ = o;
}

int SerialportTask::svc (void)
{
    ACE_DEBUG((LM_DEBUG,
                "Create zigbee serialport Service thread\n"));

    ACE_Time_Value last_time = ACE_OS::gettimeofday();
    ACE_Time_Value current_time = ACE_OS::gettimeofday();

    while( !thread_exit_flag)
    {
        ACE_Message_Block *b = 0;

        if (message_queue.dequeue_head(b) == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "faile get block from queue\n"));
            continue;
        }

        //ACE_DEBUG((LM_DEBUG, "*** get type[%x] size[%d] ****\n", b->msg_type(),
        //message_queue.message_count()));

        switch(b->msg_type())
        {
            case ZIGBEE_SERIAL_PORT_CMD_EXIT_THREAD:
            {
                thread_exit_flag = 1;
            }
            break;

            case ZIGBEE_SERIAL_PORT_CMD_CONNECT_PORT:
            {
               owner_->connecting_serial_port();
            }
            break;

            case ZIGBEE_SERIAL_PORT_CMD_SEND_REQ:
            {
               current_time = ACE_OS::gettimeofday();

               // each command cant send togather. when the time expired
               // send it
               if ( current_time.msec() > last_time.msec() + 1500 )
               {
                    owner_->send_req((unsigned char*)b->base(), b->size());

                    last_time = ACE_OS::gettimeofday();
               }
               else
               {
                    // re-enter queue to wait time expired
                    message_queue.enqueue_tail(b);
                    b = 0;
               }

            }
            break;

            case ZIGBEE_SERIAL_PORT_CMD_GET_RES:
            {
                owner_->get_response();
            }
            break;

            default:
                break;

        }

        if (b)
        {
            b->release();
        }
    }

    ACE_DEBUG((LM_DEBUG,
                "Return from zigbee serialport Service thread\n"));

    return 0;

}

//////////////////////////////////////////////////////////////////////////////

ZigbeeSerialportService *ZigbeeSerialportService::instance_ = 0;

ZigbeeSerialportService *ZigbeeSerialportService::instance()
{
    if (ZigbeeSerialportService::instance_  != 0 );
    {
        return ZigbeeSerialportService::instance_ ;
    }

    return 0;
}


ZigbeeSerialportService::ZigbeeSerialportService(CfgService *conf,
                                                 NetService *net)
:io_svc_(0),
conf_(conf),
net_(net)
{
    state_ = Idle;
    parser_ = 0;
    task_ = 0;

    ZigbeeSerialportService::instance_ = this;
}

ZigbeeSerialportService::~ZigbeeSerialportService()
{
}

int ZigbeeSerialportService::Init()
{
    parser_ = new ZigbeeSerialportParser();
    task_ = new SerialportTask();

    task_->set_owner(this);

    // active the serial port thread.
    if (task_->activate() == -1 )
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to start serialport Service thread\n"));
        return -1;
    }

    connect_to_serialport();

    return 0;
}

int ZigbeeSerialportService::Close()
{
    exit_and_wait_thread();

    if (net_)
    {
        net_->remove_handler(this);
        net_->cancel_timer(this);
    }

    ACE_DEBUG((LM_DEBUG,
            "***************** Serial port close ********************* \n"));


    if (io_svc_ != 0)
    {
        io_svc_->close();

        delete io_svc_;
        io_svc_ = 0;
    }

    if (parser_)
    {
        delete parser_;
        parser_ = 0;
    }

    if (task_)
    {
        delete task_;
        task_ = 0;
    }

    return 0;
}

int ZigbeeSerialportService::open_serial_port()
{
    if ( io_svc_ != 0 )
    {
        ACE_DEBUG((LM_DEBUG, "the serialport have been opened.\n"));
        return -1;
    }

    io_svc_ = new ACE_TTY_IO();

    ACE_DEV_Connector con;

    if (con.connect ((*io_svc_),
                   ACE_DEV_Addr (conf_->serial_port_.c_str())) == -1)
    {
        delete io_svc_;
        io_svc_ = 0;

        ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to open serial port:%p\n"),
                       conf_->serial_port_.c_str()),
                     -1);
    }

    ACE_TTY_IO::Serial_Params serial_port_params;

    serial_port_params.baudrate = conf_->baudrate_;
    serial_port_params.xonlim = 0;
    serial_port_params.xofflim = 0;
    serial_port_params.readmincharacters = 0;
    serial_port_params.readtimeoutmsec = 1000; // 10 seconds
    serial_port_params.paritymode = "none"; //"EVEN";
    serial_port_params.ctsenb = false;
    serial_port_params.rtsenb = 0;
    serial_port_params.xinenb = false;
    serial_port_params.xoutenb = false;
    serial_port_params.modem = false;
    serial_port_params.rcvenb = true;
    serial_port_params.dsrenb = false;
    serial_port_params.dtrdisable = false;
    serial_port_params.databits = 8;
    serial_port_params.stopbits = 1;

    if (io_svc_->control (ACE_TTY_IO::SETPARAMS,
                        &serial_port_params) == -1)
    {
        delete io_svc_;
        io_svc_ = 0;

        ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("failed to set %p control\n"),
                       conf_->serial_port_.c_str()),
                      -1);
    }

    state_ = Opened;

    net_->RegHandler(this, ACE_Event_Handler::READ_MASK);

    ACE_DEBUG((LM_DEBUG, "Sucessfully opened serial port:%s\n", conf_->serial_port_.c_str()));

}

int ZigbeeSerialportService::handle_input (ACE_HANDLE fd)
{
    {
        ACE_Message_Block *b = new ACE_Message_Block();
        b->msg_type(ZIGBEE_SERIAL_PORT_CMD_GET_RES);

        if (task_->message_queue.enqueue_tail(b)== -1 )
        {
            ACE_DEBUG((LM_DEBUG, "faild to input reactor block into queue\n"));
        }

        reactor()->suspend_handler(this);
    }

    return 0;
}

int ZigbeeSerialportService::handle_timeout (const ACE_Time_Value &tv,
                            const void *arg)
{
    if (state_ == Idle)
    {
        connect_to_serialport();
    }

    return 0;
}

ACE_HANDLE ZigbeeSerialportService::get_handle (void) const
{
    if (io_svc_)
    {
        return io_svc_->get_handle();
    }

    return 0;
}

int ZigbeeSerialportService::send(ZigbeeRequest  *req)
{
    {
        ACE_Message_Block *b = new ACE_Message_Block(req->size());
        ACE_OS::memcpy(b->base(), req->base(), req->size());

        b->msg_type(ZIGBEE_SERIAL_PORT_CMD_SEND_REQ);

        if (task_->message_queue.enqueue_tail(b) == -1 )
        {
            ACE_DEBUG((LM_DEBUG, "ZigbeeSerialportService faild to input node block into queue\n"));
        }

        delete req;
    }

    return 0;
}

void ZigbeeSerialportService::exit_and_wait_thread()
{
    ACE_Message_Block *b = new ACE_Message_Block();
    b->msg_type(ZIGBEE_SERIAL_PORT_CMD_EXIT_THREAD);

    task_->message_queue.enqueue_head(b);
    task_->wait();
}

void ZigbeeSerialportService::connect_to_serialport()
{
    ACE_Message_Block *b = new ACE_Message_Block();

    b->msg_type(ZIGBEE_SERIAL_PORT_CMD_CONNECT_PORT);

    task_->message_queue.enqueue_tail(b);
}

void ZigbeeSerialportService::connecting_serial_port()
{
    if (open_serial_port() == -1)
    {
        if (net_)
        {
            ACE_Time_Value timeout;
            timeout.sec(5);
            net_->schedule_timer(this, 0, timeout);
        }
    }
    else
    {
        notify_serialport_connected();
    }
}

void ZigbeeSerialportService::register_notify_node(ZigbeeNotify *n)
{
    notify_list_.push_back(n);
}

void ZigbeeSerialportService::notify_serialport_connected()
{
    notify(ZIGBEE_NOTIFY_SERIALPORT_CONNECTED);
}

void ZigbeeSerialportService::notify(unsigned short n)
{
    std::vector<ZigbeeNotify*>::iterator e;
    e = notify_list_.begin();

    for (; e != notify_list_.end(); ++e)
    {
        (*e)->notify(n);
    }
}

void ZigbeeSerialportService::send_req(unsigned char *buf, unsigned int len)
{
    dump_(buf, len);
    if (state_ == Opened)
    {
        if ( io_svc_ && io_svc_->send(buf, len) == -1)
        {
            ACE_DEBUG((LM_DEBUG,"failed to send command by serial port\n"));

            //FIXME: to handle exception
        }
    }
}

void ZigbeeSerialportService::get_response()
{
    if (io_svc_)
    {
        while (1)
        {
            unsigned char data[0xff];
            ACE_OS::memset(data, 0, 0xff);

            int n = io_svc_->recv(data, 0xff);

             // exception, FIXME: to handle this condition.
            if (n <= 0 )
            {
                if ( ACE_OS::last_error()!=EWOULDBLOCK &&
                     ACE_OS::last_error()!=EAGAIN)
                {
                   // ACE_DEBUG((LM_DEBUG,"Failed to get data from serial port\n"));
                }

                break;
            }
            else if (parser_)
            {
                ZigbeeSerialportCommand *cmd;
                ZigbeeResponse *response;

                parser_->add_data(data, n);

                while ((cmd = parser_->get_frame()) != 0)
                {
                   response = new ZigbeeResponse();
                   response->set_cmd(cmd);

                   response->get();
                }
            }
        }

        reactor()->resume_handler(this);
    }

}



