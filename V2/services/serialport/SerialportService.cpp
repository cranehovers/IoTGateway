
#include <runtime/ServiceContext.h>
#include <services/serialport/SerialportService.h>

namespace Services {
namespace SerialPort {


GWSP::Service *instance(GWSP::ServiceContext &context)
{
    return new SerialPortService(context);
}

static std::string serviceName("service.serialport");


SerialPortService::SerialPortService(GWSP::ServiceContext &context)
:Service(context)
{
}

SerialPortService::~SerialPortService()
{
}

std::string &SerialPortService::name()
{
    return serviceName;
}

bool SerialPortService::initialize()
{
    return true;
}

bool SerialPortService::unInitialize()
{
    return true;
}

bool SerialPortService::start()
{
    return true;
}

bool SerialPortService::stop()
{
    return true;
}




}
}








