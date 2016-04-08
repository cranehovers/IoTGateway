#include "Config.h"
#include "Gateway.h"
#include "AndroidService.h"


int startService()
{
    if ( IoTGateway::instance()->Init() >= 0)
    {
        IoTGateway::instance()->Start();
        IoTGateway::instance()->Stop();
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
    ACE_DEBUG((LM_DEBUG, "Stop!!!!\n"));

    IoTGateway::instance()->Stop();

    return 0;
}
