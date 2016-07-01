#include "Config.h"
#include "Gateway.h"
#include "AndroidService.h"

#include<fstream>


static output_log sPrintOutput = 0;

class PrintOutCallback : public ACE_Log_Msg_Callback
{
public:
    void log (ACE_Log_Record &log_record);
};

void PrintOutCallback::log(ACE_Log_Record &log_record)
{
    ACE_CString data = ACE_TEXT_ALWAYS_CHAR (log_record.msg_data ());

    if (sPrintOutput)
    {
        (*sPrintOutput)("%s", data.c_str ());
    }
}

void setPrintOutput(output_log output)
{
    sPrintOutput = output;
}

void init_ace_log()
{
    PrintOutCallback* printCallback = new PrintOutCallback();

    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER | ACE_Log_Msg::OSTREAM);
    ACE_LOG_MSG->set_flags (ACE_Log_Msg::MSG_CALLBACK);
    ACE_LOG_MSG->msg_callback (printCallback);
    ACE_LOG_MSG->open("gateway", ACE_Log_Msg::MSG_CALLBACK ,"gateway");
}

int initService()
{
    init_ace_log();

    return 0;
}

int startService()
{
    init_ace_log();

    if ( IoTGateway::instance()->Init() >= 0)
    {
        IoTGateway::instance()->Start();
    }
    else
    {
        ACE_DEBUG((LM_DEBUG,
                    "Failed to initialize gateway, please check...\n"));
    }

    return 0;
}

int stopService()
{
    return 0;
}
