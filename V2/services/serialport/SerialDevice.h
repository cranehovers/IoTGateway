

#ifndef SerialDevice_H
#define SerialDevice_H

#include <toolkit/ReferenceCountObject.h>
#include <string>


namespace Services {
namespace SerialPort {


class SerialDevice : public toolkit::RefCountedObject
{
public:

    SerialDevice();
    ~SerialDevice();

    void setProperty(std::string &path, int baudrate);
    ACE_HANDLE get_handle (void) const;
    bool open();
    bool close();
    bool online();
    int recv(unsigned char *buf, int n);
    int send(unsigned char *buf, int n);
    
private:

    ACE_TTY_IO  _serialport;
    std::string _path;
    int         _baudrate;
    
};


}
}



#endif //SerialDevice_H









