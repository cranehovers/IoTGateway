
/*
* Serial Port Monitor
*/

#include <stdio.h>
#include <stdlib.h>
#include "Zigbee_Serialport_Monitor.h"
#include "CfgService.h"
#include "Zigbee_Service.h"

int SerialPortMonitor::svc (void)
{

#define STATUS_PLUGIN 1
#define STATUS_NOT_PLUGIN 0

    // thie status 0, means that the serial prot not be plugined
    int status = STATUS_NOT_PLUGIN;
    int current_status = STATUS_NOT_PLUGIN;

    while (thread_exit_flag == 0)
    {
        if (conf_  != 0 && zigbee_service_ != 0)
        {
            int ttyusb_file_handle = ACE_OS::access(conf_->serial_port_.c_str(), 0);

            // the serial port not be plugined
            if (ttyusb_file_handle != 0 )
            {
                current_status = STATUS_NOT_PLUGIN;
            }
            // the serial port be plugined
            else
            {
                current_status = STATUS_PLUGIN;
            }

            // the status changed, means the serial port plugin or unplugin
            if (status !=  current_status)
            {
                status = current_status;

                ACE_DEBUG((LM_DEBUG, "The serial port status changed,%d\n", current_status));

                switch (current_status)
                {
                    case STATUS_NOT_PLUGIN:
                    {
                        zigbee_service_->serialport_unplugin();
                    }
                    break;

                    case STATUS_PLUGIN:
                    {
                        ACE_DEBUG((LM_DEBUG, "Waiting for zigbee devices register to coordinator, 30 seconds\n"));

                        ACE_OS::sleep(30);

                        if (ACE_OS::access(conf_->serial_port_.c_str(), 0) == 0)
                        {
                            zigbee_service_->serialport_plugin();
                        }
                    }
                    break;
                }
            }
        }

        // sleep 1 second, then monitor again.
        ACE_OS::sleep(2);
    }

    return 0;
}

void SerialPortMonitor::clear()
{
    thread_exit_flag = 1;

    wait();
}
