/*
* This file defines lookup Domain resource
*/

#ifndef __COAP_RD_LOOKUP_D_RESOURCE_H__
#define __COAP_RD_LOOKUP_D_RESOURCE_H__

#include <toolkit/CoAP_Resource.h>


using toolkit::CoAPWrapper;
using toolkit::CoAPResource;
using toolkit::CoAPCallback;

namespace Services {
namespace CoAPRD {


class CoAPRDLookUpDomainResource: public CoAPResource
{
public:

    CoAPRDLookUpDomainResource(CoAPWrapper* ctx);
    virtual ~CoAPRDLookUpDomainResource();
    virtual void handler_get(CoAPCallback &callback);

    void* Create();

private:
    
    
};

}
}

#endif //__COAP_RD_LOOKUP_D_RESOURCE_H__



