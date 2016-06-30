
#include <services/serialport/SerialDevice.h>

namespace Services {
namespace SerialPort {


SerialDevice::SerialDevice()
:_path("/dev/ttyUSB0"),
_baudrate(38400)
{
}

SerialDevice::~SerialDevice()
{
}

ACE_HANDLE SerialDevice::get_handle (void) const
{
    return _serialport.get_handle();
}

void SerialDevice::setProperty(std::string &path, int baudrate)
{
    _path = path;
    _baudrate = baudrate;
}

bool SerialDevice::open()
{
    ACE_DEV_Connector con;

    if (con.connect ((_serialport),
                   ACE_DEV_Addr (_path.c_str())) == -1)
    {
        return false;
        
       /* ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Failed to open serial port:%p\n"),
                       _path.c_str()),
                     false);*/
    }

    ACE_TTY_IO::Serial_Params serial_port_params;

    serial_port_params.baudrate = _baudrate;
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

    if (_serialport.control (ACE_TTY_IO::SETPARAMS,
                        &serial_port_params) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("failed to set %p control\n"),
                       _path.c_str()),
                      false);
    }

    ACE_DEBUG((LM_DEBUG, "successfully opened serial port:%s\n", _path.c_str()));

    return true;
}

bool SerialDevice::close()
{
    _serialport.close();

    return true;
}

bool SerialDevice::online()
{
    // check the dev file is existed or not to 
    // set serialport online or offline
    if (ACE_OS::access(_path.c_str(), 0) != 0) 
        return false;

    return true;
}

}
}








