#ifndef APPLICATION_H
#define APPLICATION_H

#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>
#include <runtime/ServicesRuntime.h>    
#include <services/event/EventService.h>
#include <services/event/EventNotifyHandler.h>

using Services::Event::EventService;
using Services::Event::EventNotifyHandler;


namespace GWServer{

class  GWApplication : public toolkit::RefCountedObject, public EventNotifyHandler
{
public:
	GWApplication();
	~GWApplication();

	void init(int argc, char* argv[]);
	virtual int run();
	virtual int handleEvent(int id, const ACE_Message_Block &b);

private:

	GWApplication(const GWApplication&);
	GWApplication& operator = (const GWApplication&);

	GWSP::ServicesRuntime::Ptr _servicesRuntimePtr;
	EventService::Ptr _eventServicePtr;
	
};

}


#endif // APPLICATION_H
