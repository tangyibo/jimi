
#ifndef _JIMI_THREAD_DEF_H_
#define _JIMI_THREAD_DEF_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <jimi/core/jimi_def.h>

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include <windows.h>

NS_JIMI_BEGIN

NS_JIMI_SYSTEM_BEGIN

#define HANDLE_IS_VALID(X)      ((X) != NULL && (X) != INVALID_HANDLE_VALUE)

typedef enum WAIT_FOR_OBJECT
{
    WAIT_FOR_IGNORE = IGNORE,
    WAIT_FOR_INFINITE = INFINITE,
} WAIT_FOR_OBJECT;

typedef HANDLE jm_handle;

struct Timeout
{
    static const uint32_t kInfinite = INFINITE;
    static const uint32_t kIgnore   = IGNORE;
};

struct ErrorCode
{
    static const uint32_t kErrorSuccess = ERROR_SUCCESS;
    static const uint32_t kNoError      = NO_ERROR;
};

struct WaitFor
{
    static const uint32_t kSignalled    = WAIT_OBJECT_0;
    static const uint32_t kTimeout      = WAIT_TIMEOUT;
    static const uint32_t kAbandoned    = WAIT_ABANDONED;       // For Mutex
    static const uint32_t kIoCompletion = WAIT_IO_COMPLETION;
    static const uint32_t kWaitObject_0 = WAIT_OBJECT_0;
    static const uint32_t kAbandoned_0  = WAIT_ABANDONED_0;
    static const uint32_t kFailed       = WAIT_FAILED;
};

struct SignalState
{
    static const uint32_t kUndefined    = (uint32_t)-1;
    static const uint32_t kSignalled    = TRUE;
    static const uint32_t kNoSignalled  = FALSE;
};

struct EventResetMode
{
    static const uint32_t kUndefined    = (uint32_t)-1;
    static const uint32_t kAutoReset    = FALSE;
    static const uint32_t kManualReset  = TRUE;
};

NS_JIMI_SYSTEM_END

NS_JIMI_END

#endif  /* _JIMI_THREAD_DEF_H_ */
