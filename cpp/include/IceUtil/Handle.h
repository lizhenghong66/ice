// **********************************************************************
//
// Copyright (c) 2001
// MutableRealms, Inc.
// Huntsville, AL, USA
//
// All Rights Reserved
//
// **********************************************************************

#ifndef ICE_UTIL_HANDLE_H
#define ICE_UTIL_HANDLE_H

#include <IceUtil/Config.h>
#include <algorithm>

//
// "Handle" or "smart pointer" classes for classes derived from
// IceUtil::Shared or IceUtil::SimpleShared.
//
namespace IceUtil
{

template<typename T>
class Handle
{
public:
    
    Handle(T* p = 0) :
	_ptr(p)
    {
	if (_ptr)
	{
	    _ptr->__incRef();
	}
    }
    
    template<typename Y>
    Handle(const Handle<Y>& r) :
	_ptr(r._ptr)
    {
	if (_ptr)
	{
	    _ptr->__incRef();
	}
    }

#ifdef WIN32 // COMPILERBUG: Is VC++ or GNU C++ right here???
    template<>
    Handle(const Handle<T>& r) :
#else
    Handle(const Handle& r) :
#endif
	_ptr(r._ptr)
    {
	if (_ptr)
	{
	    _ptr->__incRef();
	}
    }
    
    ~Handle()
    {
	if (_ptr)
	{
	    _ptr->__decRef();
	}
    }
    
    Handle& operator=(T* p)
    {
	if (_ptr != p)
	{
	    if (p)
	    {
		p->__incRef();
	    }

	    if (_ptr)
	    {
		_ptr->__decRef();
	    }
	    
	    _ptr = p;
	}
	return *this;
    }
        
    template<typename Y>
    Handle& operator=(const Handle<Y>& r)
    {
	if (_ptr != r._ptr)
	{
	    if (r._ptr)
	    {
		r._ptr->__incRef();
	    }

	    if (_ptr)
	    {
		_ptr->__decRef();
	    }
	    
	    _ptr = r._ptr;
	}
	return *this;
    }

#ifdef WIN32 // COMPILERBUG: Is VC++ or GNU C++ right here???
    template<>
    Handle& operator=(const Handle<T>& r)
#else
    Handle& operator=(const Handle& r)
#endif
    {
	if (_ptr != r._ptr)
	{
	    if (r._ptr)
	    {
		r._ptr->__incRef();
	    }

	    if (_ptr)
	    {
		_ptr->__decRef();
	    }
	    
	    _ptr = r._ptr;
	}
	return *this;
    }
        
    template<class Y>
    static Handle dynamicCast(const Handle<Y>& r)
    {
	return Handle(dynamic_cast<T*>(r._ptr));
    }

    template<class Y>
    static Handle dynamicCast(Y* p)
    {
	return Handle(dynamic_cast<T*>(p));
    }

    typedef T element_type;
    
    T* get() const { return _ptr; }
    T* operator->() const { return _ptr; }
    operator bool() const { return _ptr ? true : false; }

    void swap(Handle& other) { std::swap(_ptr, other._ptr); }

#ifndef WIN32 // COMPILERBUG: VC++ 6.0 doesn't understand this
 
    template<typename Y> friend class Handle;

protected:

#endif

    T* _ptr;
};

template<typename T, typename U>
inline bool operator==(const Handle<T>& a, const Handle<U>& b)
{
    T* ap = a.get();
    U* bp = b.get();
    if (ap == bp)
    {
	return true;
    }
    else if (!ap)
    {
	assert(bp);
	return false;
    }
    else
    {
	return *ap == *bp;
    }
}

template<typename T, typename U>
inline bool operator<(const Handle<T>& a, const Handle<U>& b)
{
    T* ap = a.get();
    U* bp = b.get();
    if (!ap || !bp)
    {
	if (!ap && bp)
	{
	    return true;
	}
	else
	{
	    return false;
	}
    }
    else
    {
	return *a.get() < *b.get();
    }
}

}

#endif
