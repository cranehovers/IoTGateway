
#include <services/serialport/SerialDevice.h>
#include <services/serialport/SerialportEventHandler.h>

    
namespace Services {
namespace SerialPort {

SerialportEventHandler::SerialportEventHandler(SerialDevice &device)
:_serialDevice(device)
{
}


SerialportEventHandler::~SerialportEventHandler()
{
}

ACE_HANDLE SerialportEventHandler::get_handle (void) const
{
    return _serialDevice.get_handle();
}


int SerialportEventHandler::handle_input (ACE_HANDLE fd)
{
    return 0;
}

int SerialportEventHandler::handle_timeout (const ACE_Time_Value &tv,
                                            const void *arg)
{
    return 0;
}

}
}






