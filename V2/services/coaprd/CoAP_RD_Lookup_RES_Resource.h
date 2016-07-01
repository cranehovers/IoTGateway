/*
* This file defines lookup RES resource
*/

#ifndef __COAP_RD_LOOKUP_RES_RESOURCE_H__
#define __COAP_RD_LOOKUP_RES_RESOURCE_H__

#include <toolkit/CoAP_Resource.h>


using toolkit::CoAPWrapper;
using toolkit::CoAPResource;

namespace Services {
namespace CoAPRD {



class CoAPRDLookUpResResource: public CoAPResource
{
public:

    CoAPRDLookUpResResource(CoAPWrapper* ctx);
    virtual ~CoAPRDLookUpResResource();

    void* Create();

private:
    
    
};

}
}


#endif //__COAP_RD_LOOKUP_RES_RESOURCE_H__



