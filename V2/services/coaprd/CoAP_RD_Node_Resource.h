/*
* This file defines RD Node Resource
*/

#ifndef __COAP_RD_NODE_RESOURCE_H__
#define __COAP_RD_NODE_RESOURCE_H__


#include <toolkit/CoAP_Resource.h>


using toolkit::CoAPWrapper;
using toolkit::CoAPCallback;
using toolkit::CoAPResource;


namespace Services {
namespace CoAPRD {

class CoAPRDNodeResource : public CoAPResource
{
public:

    CoAPRDNodeResource(CoAPWrapper* ctx);
    virtual ~CoAPRDNodeResource();


private:

};

}
}


#endif //__COAP_RD_NODE_RESOURCE_H__



