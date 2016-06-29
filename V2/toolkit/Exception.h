#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>


namespace toolkit{

class Exception: public std::exception
{
public:
    Exception(const std::string& msg, int code = 0);
    Exception(const std::string& msg, const std::string& arg, int code = 0);
    Exception(const std::string& msg, const Exception& nested, int code = 0);
    Exception(const Exception& exc);

    ~Exception() throw();

    Exception& operator = (const Exception& exc);

    virtual const char* name() const throw();
    virtual const char* className() const throw();
    virtual const char* what() const throw();
    const Exception* nested() const;
    const std::string& message() const;
    int code() const;
    std::string displayText() const;

    virtual Exception* clone() const;
    virtual void rethrow() const;

protected:
    Exception(int code = 0);

    void message(const std::string& msg);

    void extendedMessage(const std::string& arg);

private:
    std::string _msg;
    Exception*  _pNested;
    int			_code;
};

inline const Exception* Exception::nested() const
{
	return _pNested;
}


inline const std::string& Exception::message() const
{
	return _msg;
}


inline void Exception::message(const std::string& msg)
{
	_msg = msg;
}


inline int Exception::code() const
{
	return _code;
}


#define GWSP_DECLARE_EXCEPTION_CODE(CLS, BASE, CODE) \
	class CLS: public BASE														\
	{																				\
	public:																			\
		CLS(int code = CODE);														\
		CLS(const std::string& msg, int code = CODE);								\
		CLS(const std::string& msg, const std::string& arg, int code = CODE);		\
		CLS(const std::string& msg, const Exception& exc, int code = CODE);	\
		CLS(const CLS& exc);														\
		~CLS() throw();																\
		CLS& operator = (const CLS& exc);											\
		const char* name() const throw();											\
		const char* className() const throw();										\
		toolkit::Exception* clone() const;												\
		void rethrow() const;														\
	};

#define GWSP_DECLARE_EXCEPTION(CLS, BASE) \
	GWSP_DECLARE_EXCEPTION_CODE(CLS, BASE, 0)

#define GWSP_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)													\
	CLS::CLS(int code): BASE(code)																	\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, int code): BASE(msg, code)										\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const std::string& arg, int code): BASE(msg, arg, code)		\
	{																								\
	}																								\
	CLS::CLS(const std::string& msg, const toolkit::Exception& exc, int code): BASE(msg, exc, code)	\
	{																								\
	}																								\
	CLS::CLS(const CLS& exc): BASE(exc)																\
	{																								\
	}																								\
	CLS::~CLS() throw()																				\
	{																								\
	}																								\
	CLS& CLS::operator = (const CLS& exc)															\
	{																								\
		BASE::operator = (exc);																		\
		return *this;																				\
	}																								\
	const char* CLS::name() const throw()															\
	{																								\
		return NAME;																				\
	}																								\
	const char* CLS::className() const throw()														\
	{																								\
		return typeid(*this).name();																\
	}																								\
	toolkit::Exception* CLS::clone() const																\
	{																								\
		return new CLS(*this);																		\
	}																								\
	void CLS::rethrow() const																		\
	{																								\
		throw *this;																				\
	}


//
// Standard exception classes
//
GWSP_DECLARE_EXCEPTION(LogicException, Exception)
GWSP_DECLARE_EXCEPTION(AssertionViolationException, LogicException)
GWSP_DECLARE_EXCEPTION(NullPointerException, LogicException)

}

#endif // EXCEPTION_H
