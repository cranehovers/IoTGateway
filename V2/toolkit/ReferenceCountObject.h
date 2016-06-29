#ifndef REFERENCECOUNTOBJECT_H
#define REFERENCECOUNTOBJECT_H

#include <toolkit/ACE_Header_File.h>

namespace toolkit {

class RefCountedObject
{
public:
    RefCountedObject();
    void duplicate() const;
    void release() const throw();
    int referenceCount() const;

protected:
    virtual ~RefCountedObject();

private:
    RefCountedObject(const RefCountedObject&);
    RefCountedObject& operator = (const RefCountedObject&);

    typedef ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long> UnsignedAtomicLong;

    mutable UnsignedAtomicLong _counter;
};

inline int RefCountedObject::referenceCount() const
{
	return _counter.value();
}


inline void RefCountedObject::duplicate() const
{
	++_counter;
}


inline void RefCountedObject::release() const throw()
{
    if (--_counter == 0) delete this;
}

}

#endif // REFERENCECOUNTOBJECT_H
