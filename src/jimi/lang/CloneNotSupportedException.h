
#ifndef _JIMI_LANG_CLONENOTSUPPORTEDEXCEPTION_H_
#define _JIMI_LANG_CLONENOTSUPPORTEDEXCEPTION_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <jimi/core/jimi_def.h>
#include <jimi/lang/Exception.h>

NS_JIMI_BEGIN

class CloneNotSupportedException : public Exception
{
public:
    CloneNotSupportedException() {};
    ~CloneNotSupportedException() {};
};

NS_JIMI_END

#endif  /* _JIMI_LANG_CLONENOTSUPPORTEDEXCEPTION_H_ */