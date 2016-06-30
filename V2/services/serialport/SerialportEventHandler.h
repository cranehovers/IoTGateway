

#ifndef SerialportEventHandler_H
#define SerialportEventHandler_H

#include <toolkit/ReferenceCountObject.h>
#include <string>


namespace Services {
namespace SerialPort {


class SerialDevice;

class SerialportEventHandler :public toolkit::RefCountedObject, public ACE_Event_Handler
{
public:
    
    SerialportEventHandler(SerialDevice &device);
    ~SerialportEventHandler();

    virtual ACE_HANDLE get_handle (void) const;
    virtual int handle_input (ACE_HANDLE fd);
    virtual int handle_timeout (const ACE_Time_Value &tv,
                                const void *arg);


private:

    SerialDevice &_serialDevice;
};


}
}



#endif //SerialportEventHandler_H




