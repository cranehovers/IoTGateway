/*
* This file defines main method for Gateway
*/

#include "Config.h"
#include "Gateway.h"

/*
* Gateway uses the ace log system to record it's log
* and debug
*/
static int Initialize_ACE_Log()
{
    return 0;
}

void init_ace_log()
{
}

class Startx :public ACE_Task_Base
{
public:

    void start()
    {
        activate();
    }

    virtual int svc (void)
    {
        if ( IoTGateway::instance()->Init() >= 0)
        {
            IoTGateway::instance()->Start();
        }
        else
        {
            ACE_DEBUG((LM_DEBUG,
                        "Failed to initialize gateway, please check...\n"));
        }

    }
};

int main(int argc, char** argv)
{
    Initialize_ACE_Log();

    Startx startx;
    startx.start();

    char c;
    while (c = getchar())
    {
        if (c == 'q' || c == 'Q')
        {
            ACE_DEBUG((LM_DEBUG, "********STOP********\n"));

            IoTGateway::instance()->Stop();
            break;
        }
    }

    return 0;
}






