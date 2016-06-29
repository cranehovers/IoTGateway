
#include <toolkit/ReferenceCountObject.h>

namespace toolkit{

RefCountedObject::RefCountedObject(): _counter(1)
{
}


RefCountedObject::~RefCountedObject()
{
}

}

