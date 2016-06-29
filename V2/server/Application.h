#ifndef APPLICATION_H
#define APPLICATION_H

#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>
#include <runtime/ServicesRuntime.h>

namespace GWServer{

class  GWApplication : public toolkit::RefCountedObject
{
public:
	GWApplication();
	~GWApplication();

	void init(int argc, char* argv[]);
	virtual int run();

private:

	GWApplication(const GWApplication&);
	GWApplication& operator = (const GWApplication&);


	typedef toolkit::AutoPtr<GWSP::ServicesRuntime> ServicesRuntimePtr;

	ServicesRuntimePtr _servicesRuntimePtr;
};

}


#endif // APPLICATION_H
