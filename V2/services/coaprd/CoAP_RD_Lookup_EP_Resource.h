/*
* This file defines lookup  EP resource
*/

#ifndef __COAP_RD_LOOKUP_EP_RESOURCE_H__
#define __COAP_RD_LOOKUP_EP_RESOURCE_H__

#include <toolkit/CoAP_Resource.h>


using toolkit::CoAPWrapper;
using toolkit::CoAPResource;
using toolkit::CoAPCallback;


namespace Services {
namespace CoAPRD {



class CoAPRDLookUpEPResource: public CoAPResource
{
public:

    CoAPRDLookUpEPResource(CoAPWrapper* ctx);
    virtual ~CoAPRDLookUpEPResource();
    virtual void handler_get(CoAPCallback &callback);

    void* Create();

private:
    
    
};


}
}


#endif //__COAP_RD_LOOKUP_EP_RESOURCE_H__




