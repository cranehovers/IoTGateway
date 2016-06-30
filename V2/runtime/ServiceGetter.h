
#ifndef ServiceGetter_H
#define ServiceGetter_H


#include <toolkit/ReferenceCountObject.h>
#include <toolkit/AutoPtr.h>
#include <toolkit/Exception.h>
#include <runtime/ServiceContext.h>
#include <runtime/ServiceRepository.h>
#include <runtime/Service.h>


#include <string>

namespace GWSP {

class ServiceGetter
{
public:
    
	template <typename Svc>
	static toolkit::AutoPtr<Svc> findByName(ServiceContext &context, 
	                                        std::string& name)
	{
		Service &service = context.repo().get(name);

        //get service.
		if (service.name().compare(name) == 0)
		{
			Svc* pCasted = dynamic_cast<Svc*>(&service);			
			return toolkit::AutoPtr<Svc>(pCasted, true);
		}
		else
		{
		    throw toolkit::NullPointerException("not found");
		}
	}	
};

}

#endif //ServiceGetter_H

