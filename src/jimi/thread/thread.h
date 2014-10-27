
#ifndef _JIMI_SYSTEM_THREAD_H_
#define _JIMI_SYSTEM_THREAD_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <jimi/core/jimi_def.h>
#include <jimi/thread/ThreadDef.h>
#include <jimi/log/log.h>

#if JIMI_IS_WINDOWS

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <process.h>

#else

#ifndef CREATE_SUSPENDED
#define CREATE_SUSPENDED        0x00000004
#endif

#endif  /* JIMI_IS_WINDOWS */

NS_JIMI_BEGIN

NS_JIMI_SYSTEM_BEGIN

#define TERMINATE_WAIT_TIME             5000

enum ThreadStatusShift
{
    // Thread extra status
    kBackgroundStatusShift      = 31,
    kJoinStatusShift            = 30,

    // Thread status
    kAliveStatusShift           = 23,
    kRunningStatusShift         = 22,
};

enum ThreadStatusMask
{
    // Thread extra status mask
    kBackgroundStatusMask       = (1UL << kBackgroundStatusShift),
    kJoinStatusMask             = (1UL << kJoinStatusShift),

    // Thread status mask
    kAliveStatusMask            = (1UL << kAliveStatusShift),
    kRunningStatusMask          = (1UL << kRunningStatusShift),

    kAliveAndRunningStatusMask  = kAliveStatusMask | kRunningStatusMask,

    kThreadExtraStatusMask      =  ((1UL << kBackgroundStatusShift) | (1UL << kJoinStatusShift)),
    kThreadStatusMask           =  ~kThreadExtraStatusMask,

    kThreadStatusValueMask      =  0x0000FFFFUL,
};

#define THREAD_STATUS_MASK              (ThreadStatusMask::kThreadStatusMask)

#define THREAD_IS_BACKGROUND(status)    (((status) & kBackgroundStatusMask) != 0)
#define THREAD_IS_JOINING(status)       (((status) & kJoinStatusMask) != 0)
#define THREAD_IS_ALIVE(status)         (((status) & kAliveStatusMask) != 0)
#define THREAD_IS_RUNNING(status)       (((status) & kRunningStatusMask) != 0)

#define GET_THREAD_STATUS(status)       ((status) & ThreadStatusMask::kThreadStatusMask)
#define GET_THREAD_EXTRA_STATUS(status) ((status) & ThreadStatusMask::kThreadExtraStatusMask)

#define GET_THREAD_STATUS_VALUE(status) ((status) & ThreadStatusMask::kThreadStatusValueMask)

#define THREAD_IS_ALIVE_OR_RUNNING(status) \
    (((status) & kAliveAndRunningStatusMask) != 0)

#define THREAD_IS_ALIVE_AND_RUNNING(status) \
    (((status) & kAliveAndRunningStatusMask) == kAliveAndRunningStatusMask)

#define MAKE_THREAD_STATUS(bAlive, bRunning, nStatus)   \
    (((((bAlive) & 1UL) << kAliveStatusShift) | (((bRunning) & 1UL) << kRunningStatusShift)) + (nStatus))

#define MAKE_THREAD_EXTRA_STATUS(bAlive, bRunning, bJoin, bBackground, nStatus) \
    (MAKE_THREAD_STATUS(bAlive, bRunning, nStatus)  \
        | (((bJoin) & 1UL) << kJoinStatusShift)     \
        | (((bBackground) & 1UL) << kBackgroundStatusShift))

enum ThreadDef
{
    /* Abort()或Terminate()终止线程时的等待时间. 默认值: 5000毫秒. */
    kTerminateWaitTime  = 5000,
};

enum ThreadStatusDef
{
    kThreadStatusFirst = 0,

    kThreadStatus_Unknown = kThreadStatusFirst,

    // All Unrunning Status
    kThreadStatus_UnStarted,
    kThreadStatus_Stopped,
    kThreadStatus_Aborted,

    // All Alive Status
    kThreadStatus_StartPending,
    kThreadStatus_StopPending,
    kThreadStatus_AbortPending,
    kThreadStatus_ClosePending,

    // All Running Status
    kThreadStatus_Running,
    kThreadStatus_Suspended,
    kThreadStatus_SuspendPending,
    kThreadStatus_ResumePending,
    kThreadStatus_ThreadProcOver,

    kThreadStatusLast,
};

static char * s_szThreadStatusString[] = {
    "Unknown",
    "UnStarted",
    "Stopped",
    "Aborted",
    "StartPending",
    "StopPending",
    "AbortPending",
    "ClosePending",
    "Running",
    "Suspended",
    "SuspendPending",
    "ResumePending",
    "ThreadProcOver",
    "ThreadStatusLast"
};

typedef struct ThreadStatus
{
public:
    static const uint32_t kUnknown          = MAKE_THREAD_STATUS(0, 0, kThreadStatus_Unknown);

    // All Unrunning Status
    static const uint32_t kUnStarted        = MAKE_THREAD_STATUS(0, 0, kThreadStatus_UnStarted);
    static const uint32_t kStopped          = MAKE_THREAD_STATUS(0, 0, kThreadStatus_Stopped);
    static const uint32_t kAborted          = MAKE_THREAD_STATUS(0, 0, kThreadStatus_Aborted);

    // All Alive Status
    static const uint32_t kStartPending     = MAKE_THREAD_STATUS(1, 0, kThreadStatus_StartPending);
    static const uint32_t kStopPending      = MAKE_THREAD_STATUS(1, 0, kThreadStatus_StopPending);
    static const uint32_t kAbortPending     = MAKE_THREAD_STATUS(1, 0, kThreadStatus_AbortPending);
    static const uint32_t kClosePending     = MAKE_THREAD_STATUS(1, 0, kThreadStatus_ClosePending);

    // All Running Status
    static const uint32_t kRunning          = MAKE_THREAD_STATUS(1, 1, kThreadStatus_Running);
    static const uint32_t kSuspended        = MAKE_THREAD_STATUS(1, 1, kThreadStatus_Suspended);
    static const uint32_t kSuspendPending   = MAKE_THREAD_STATUS(1, 1, kThreadStatus_SuspendPending);
    static const uint32_t kResumePending    = MAKE_THREAD_STATUS(1, 1, kThreadStatus_ResumePending);
    static const uint32_t kThreadProcOver   = MAKE_THREAD_STATUS(1, 1, kThreadStatus_ThreadProcOver);
} ThreadStatus;

typedef struct ThreadExtraStatus
{
public:
    static const uint32_t kUnknown          = MAKE_THREAD_EXTRA_STATUS(0, 0, 0, 0, kThreadStatus_Unknown);

    // wait, for and join
    static const uint32_t kWaitSleepJoin    = MAKE_THREAD_EXTRA_STATUS(0, 0, 1, 0, kThreadStatus_Unknown);
    // background thread
    static const uint32_t kBackground       = MAKE_THREAD_EXTRA_STATUS(0, 0, 0, 1, kThreadStatus_Unknown);
} ThreadExtraStatus;

typedef struct ThreadInitFlag
{
public:
    static const uint32_t kCreateDefault    = 0;
    static const uint32_t kCreateSuspended  = CREATE_SUSPENDED;
} ThreadInitFlag;

typedef void (*thread_proc_t)(void *lpParam);

#if JIMI_IS_WINDOWS

/**
 * the Thread base class
 */
template <class T>
class ThreadBase
{
public:
    typedef jm_handle_t thread_handle_t;
    typedef jm_handle_t handle_t;
    typedef uint32_t    thread_status_t;
    typedef unsigned    thread_id_t;
    typedef uint32_t    affinity_t;

    // 工作者线程的线程参数
    typedef struct Thread_Params
    {
        ThreadBase *pThread;        // 传入的Thread指针
        void       *pObject;        // 传入的对象指针
        int         nThreadIdx;     // 线程Idx编号, 从0计起
    } THREAD_PARAMS, *PTHREAD_PARAMS;

    ThreadBase(void);
    ThreadBase(thread_proc_t thread_proc) { pThreadProc = thread_proc; }
    ~ThreadBase(void);

private:
    explicit ThreadBase(const ThreadBase &) {}
    void operator = (const ThreadBase &) {}

public:
    void Destroy();
    void OnDestroy();

    thread_proc_t GetThreadProc() { return pThreadProc; }
    bool SetThreadProc(thread_proc_t thread_proc);

    operator thread_handle_t     ()     { return hThread;  }
    thread_handle_t * operator & ()     { return &hThread; }
    thread_handle_t & GetThreadHandle() { return hThread;  }
    thread_id_t GetThreadId()           { return nThreadId; }

    thread_handle_t GetCurrentThread()  { return (thread_handle_t)::GetCurrentThread();  }
    thread_id_t GetCurrentThreadId()    { return ::GetCurrentThreadId(); }

    bool IsValid();

    bool IsAlive();
    bool IsRunning();
    bool IsAliveOrRunning();
    bool IsAliveAndRunning();
    bool IsSuspended();

    bool IsBackground();
    bool IsJoining();

    thread_status_t GetThreadState()        { return nStatus; }
    thread_status_t GetThreadExtraState()   { return nExtraStatus; }
    thread_status_t GetThreadFullState()    { return nStatus | nExtraStatus; }

    thread_status_t GetSafeThreadState()        { return GET_THREAD_STATUS(nStatus); }
    thread_status_t GetSafeThreadExtraState()   { return GET_THREAD_EXTRA_STATUS(nExtraStatus); }
    thread_status_t GetSafeThreadFullState()    {
        return GET_THREAD_STATUS(nStatus) | GET_THREAD_EXTRA_STATUS(nExtraStatus);
    }

    thread_status_t GetThreadStateValue()   { return GET_THREAD_STATUS_VALUE(nStatus); }
    thread_status_t GetThreadStateValue(thread_status_t nSpecStatus) {
        return GET_THREAD_STATUS_VALUE(nSpecStatus);
    }

    void SetThreadState(thread_status_t nNewStatus) {
        jmLog.info("ThreadBase<T>::SetThreadState():");
        jmLog.info("newStatus = 0x%08X(%s), oldStatus = 0x%08X(%s).",
                   nNewStatus, GetThreadStateString(nNewStatus),
                   nStatus, GetThreadStateString(nStatus));
        nStatus = nNewStatus;
    }
    void SetThreadExtraState(thread_status_t nNewExtraStatus) {
        jmLog.info("ThreadBase<T>::SetThreadExtraState():");
        jmLog.info("newStatus = 0x%08X(%s), oldStatus = 0x%08X(%s).",
                   nNewExtraStatus, GetThreadStateString(nNewExtraStatus),
                   nExtraStatus, GetThreadStateString(nExtraStatus));
        nExtraStatus = nNewExtraStatus;
    }

    void SetSafeThreadState(thread_status_t nNewStatus) {
        nStatus = GET_THREAD_STATUS(nNewStatus);
    }
    void SetSafeThreadExtraState(thread_status_t nNewExtraStatus) {
        nExtraStatus = GET_THREAD_EXTRA_STATUS(nNewExtraStatus);
    }
    void SetSafeThreadFullState(thread_status_t nNewFullStatus) {
        nStatus = GET_THREAD_STATUS(nNewFullStatus);
        nExtraStatus = GET_THREAD_EXTRA_STATUS(nNewFullStatus);
    }

    void AddThreadExtraState(thread_status_t nAddExtraStatus) {
        nExtraStatus |= GET_THREAD_EXTRA_STATUS(nAddExtraStatus);
    }
    void RemoveThreadExtraState(thread_status_t nRemoveExtraStatus) {
        nExtraStatus &= GET_THREAD_EXTRA_STATUS(~nRemoveExtraStatus);
    }

    char * GetThreadStateString() {
        int index = GET_THREAD_STATUS_VALUE(nStatus);
        index = JIMI_MIN(index, kThreadStatusLast - 1);
        return s_szThreadStatusString[index];
    }

    char * GetThreadStateString(thread_status_t nSpecStatus) {
        int index = GET_THREAD_STATUS_VALUE(nSpecStatus);
        index = JIMI_MIN(index, kThreadStatusLast - 1);
        return s_szThreadStatusString[index];
    }

    thread_status_t Start(void *pObject = NULL,
                          unsigned uInitflag = ThreadInitFlag::kCreateDefault);
    thread_status_t Stop(uint32_t uWaitTime = Timeout::kTerminateWaitTime,
                         uint32_t uExitCode = uint32_t(-1));

    thread_status_t Wait(handle_t hObject, uint32_t uWaitTime = Timeout::kInfinite);
    thread_status_t Join(uint32_t uWaitTime = Timeout::kInfinite);

    thread_status_t Suspend();
    thread_status_t Resume();

    thread_status_t Interrupt(uint32_t uWaitTime = Timeout::kTerminateWaitTime,
                              uint32_t uExitCode = 0);

    thread_status_t Abort(uint32_t uWaitTime = Timeout::kTerminateWaitTime,
                          uint32_t uExitCode = 0);
    thread_status_t Terminate(uint32_t uWaitTime = Timeout::kTerminateWaitTime,
                              uint32_t uExitCode = 0);

    int  GetThreadPriority() { return ::GetThreadPriority(hThread); }
    bool SetThreadPriority(int nNewPriority) {
        return (bool)::SetThreadPriority(hThread, nNewPriority);
    }

    affinity_t GetProcessorAffinity();
    affinity_t GetThreadAffinity();

    affinity_t SetProcessorAffinity();
    affinity_t SetThreadAffinity();

    static int32_t SpinWait(int32_t iterations);
    static void Sleep(uint32_t uMilliSecs);

    void JIMI_WINAPI ThreadProc(void *lpParam);

    static unsigned JIMI_WINAPI ThreadProcBase(void *lpParam);

protected:
    thread_proc_t   pThreadProc;

private:
    thread_handle_t hThread;
    thread_status_t nStatus;
    thread_status_t nExtraStatus;
    thread_id_t     nThreadId;
};

template <class T>
ThreadBase<T>::ThreadBase(void)
: pThreadProc(NULL)
, hThread(NULL)
, nStatus(ThreadStatus::kUnknown)
, nExtraStatus(ThreadExtraStatus::kUnknown)
, nThreadId(0)
{
}

template <class T>
ThreadBase<T>::~ThreadBase(void)
{
    this->Destroy();
}

template <class T>
void ThreadBase<T>::Destroy()
{
    T *pT = static_cast<T *>(this);
    pT->OnDestroy();

    if (IsValid()) {
        if (IsAlive())
            this->Terminate();
        if (IsValid())
            ::CloseHandle(hThread);
        hThread = NULL;
        nThreadId = 0;
        nStatus = ThreadStatus::kUnknown;
    }
}

template <class T>
void ThreadBase<T>::OnDestroy()
{
    jmLog.info("ThreadBase<T>::OnDestroy(), error = %d.", ::GetLastError());
}

template <class T>
bool ThreadBase<T>::IsValid()
{
    return HANDLE_IS_VALID(hThread);
}

template <class T>
bool ThreadBase<T>::IsAlive()
{
    return IsValid() && THREAD_IS_ALIVE(nStatus);
}

template <class T>
bool ThreadBase<T>::IsRunning()
{
    return IsValid() && THREAD_IS_RUNNING(nStatus);
}

template <class T>
bool ThreadBase<T>::IsAliveOrRunning()
{
    return IsValid() && THREAD_IS_ALIVE_OR_RUNNING(nStatus);
}

template <class T>
bool ThreadBase<T>::IsAliveAndRunning()
{
    return IsValid() && THREAD_IS_ALIVE_AND_RUNNING(nStatus);
}

template <class T>
bool ThreadBase<T>::IsSuspended()
{
    return IsAliveAndRunning()
        && (this->GetThreadState() == ThreadStatus::kSuspended
            || this->GetThreadState() == ThreadStatus::kSuspendPending);
}

template <class T>
bool ThreadBase<T>::IsBackground()
{
    return THREAD_IS_BACKGROUND(nExtraStatus);
}

template <class T>
bool ThreadBase<T>::IsJoining()
{
    return THREAD_IS_JOINING(nExtraStatus);
}

template <class T>
bool ThreadBase<T>::SetThreadProc(thread_proc_t thread_proc)
{
    if ((!IsAlive()) && (!IsRunning())) {
        pThreadProc = thread_proc;
        return true;
    }
    return false;
}

template <class T>
void JIMI_WINAPI ThreadBase<T>::ThreadProc(void *lpParam)
{
    // Do nothing !!
    jmLog.info("ThreadBase<T>::ThreadProc().");
}

/* static */
template <class T>
unsigned JIMI_WINAPI ThreadBase<T>::ThreadProcBase(void *lpParam)
{
    jmLog.info("ThreadBase<T>::ThreadProcBase() Enter.");
    jmLog.info("ThreadBase<T>::ThreadProcBase(), lpParam = 0x%08X.", lpParam);

    THREAD_PARAMS *pParam = (THREAD_PARAMS *)lpParam;
    ThreadBase<T> *pThread = NULL;
    void *pObject= NULL;
    if (pParam) {
        pThread = pParam->pThread;
        pObject = pParam->pObject;

        if (pThread && pThread->IsValid())
            pThread->SetThreadState(ThreadStatus::kRunning);

        T *pThis = static_cast<T *>(pThread);
        thread_proc_t pThreadProc = pThis->GetThreadProc();
        if (pThreadProc != NULL)
            pThreadProc(lpParam);
        else
            pThis->ThreadProc(lpParam);

        if (pParam)
            delete pParam;

        //ThreadBase<T>::Sleep(3000);
    }
    jmLog.info("ThreadBase<T>::ThreadProcBase() Over.");

    if (pThread && pThread->IsValid()) {
        //pThread->SetThreadState(ThreadStatus::kThreadProcOver);
        pThread->SetThreadState(ThreadStatus::kStopped);
    }
    return 0;
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Start(void *pObject /* = NULL */,
                                                             unsigned uInitflag /* = ThreadInitFlag::kCreateDefault */)
{
    jmLog.info("ThreadBase<T>::Start() Enter.");

    if (!IsValid()) {
        if (!IsAliveOrRunning()) {
            THREAD_PARAMS *pParams = new THREAD_PARAMS;
            pParams->pThread = (ThreadBase<T> *)this;
            pParams->pObject = pObject;
            pParams->nThreadIdx = 0;
            hThread = (thread_handle_t)::_beginthreadex(NULL, 0, &ThreadBase<T>::ThreadProcBase,
                (void *)pParams, uInitflag, (unsigned *)&nThreadId);
            if (HANDLE_IS_VALID(hThread)) {
                if (uInitflag == ThreadInitFlag::kCreateSuspended)
                    this->SetThreadState(ThreadStatus::kSuspended);
                else
                    this->SetThreadState(ThreadStatus::kStartPending);
            }
        }
        else {
            jmLog.info("ThreadBase<T>::Start() Thread is running or alive, nThread = 0x%08X.", hThread);
        }
    }
    else {
        jmLog.info("ThreadBase<T>::Start() Thread already exists, nThread = 0x%08X.", hThread);
    }

    jmLog.info("ThreadBase<T>::Start() Over.");
    return this->GetThreadState();
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Stop(uint32_t uWaitTime /* = Timeout::kTerminateWaitTime */,
                                                            uint32_t nExitCode /* = -1 */)
{
    bool bThreadStartted = false;
    jmLog.info("ThreadBase<T>::Stop() Enter.");
    if (IsValid()) {
        if (IsAliveOrRunning()) {
            bThreadStartted = true;
            this->Terminate(uWaitTime, nExitCode);
            jmLog.info("ThreadBase<T>::Stop() successed.");
        }
    }
    if (!bThreadStartted)
        jmLog.info("ThreadBase<T>::Stop() Thread is not startted, nThread = 0x%08X.", hThread);
    jmLog.info("ThreadBase<T>::Stop() Over.");
    return this->GetThreadState();
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Suspend()
{
    if (!IsSuspended()) {
        thread_status_t oldStatus = this->GetThreadState();
        this->SetThreadState(ThreadStatus::kSuspendPending);
        if (::SuspendThread(hThread) != (DWORD)(-1))
            this->SetThreadState(ThreadStatus::kSuspended);
        else
            this->SetThreadState(oldStatus);
    }
    return this->GetThreadState();
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Resume()
{
    if (IsSuspended()) {
        thread_status_t oldStatus = this->GetThreadState();
        this->SetThreadState(ThreadStatus::kResumePending);
        if (::ResumeThread (hThread) != (DWORD)(-1))
            this->SetThreadState(ThreadStatus::kRunning);
        else
            this->SetThreadState(oldStatus);
    }
    return this->GetThreadState();
}


template <class T>
int32_t ThreadBase<T>::SpinWait(int32_t iterations)
{
    volatile int32_t sum = 0;
    int32_t i;
    for (i = 0; i < iterations; ++i)
        sum += i;

    return sum;
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Wait(handle_t hObject,
                                                            uint32_t uWaitTime /* = Timeout::kInfinite */)
{
    jmLog.info("ThreadBase<T>::Wait() Enter.");
    static int counter = 0;
    if (HANDLE_IS_VALID(hObject)) {
        if (::WaitForSingleObject(hObject, 10) != WAIT_OBJECT_0)
            this->AddThreadExtraState(ThreadStatus::kWaitSleepJoin);
        while (::WaitForSingleObject(hObject, uWaitTime) != WAIT_OBJECT_0) {
            ThreadBase<T>::Sleep(1);
            counter++;
            if (counter > 5)
                break;
        }
        this->RemoveThreadExtraState(ThreadStatus::kWaitSleepJoin);
        if (::WaitForSingleObject(hObject, 10) == WAIT_OBJECT_0) {
            // wait for success
        }
        else {
            // wait for failed
        }
    }

    jmLog.info("ThreadBase<T>::Wait() Over.");
    return this->GetThreadFullState();
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Join(uint32_t uWaitTime /* = Timeout::kInfinite */)
{
    jmLog.info("ThreadBase<T>::Join() Enter.");    
    if (IsValid()) {
        if (::WaitForSingleObject(hThread, 10) != WAIT_OBJECT_0)
            this->AddThreadExtraState(ThreadExtraStatus::kWaitSleepJoin);
        int failed_counter = 0;
        while (::WaitForSingleObject(hThread, uWaitTime) != WAIT_OBJECT_0) {
            ThreadBase<T>::Sleep(1);
            if (uWaitTime != Timeout::kInfinite) {
                failed_counter++;
                if (failed_counter > 5)
                    break;
            }
        }
        this->RemoveThreadExtraState(ThreadExtraStatus::kWaitSleepJoin);
        if (IsValid()) {
            thread_status_t oldStatus = this->GetThreadState();
            bool bStopped = false;
            if (IsAliveOrRunning())
                this->SetThreadState(ThreadStatus::kStopPending);
            if (::WaitForSingleObject(hThread, 10) == WAIT_OBJECT_0) {
                if (IsValid()) {
                    this->SetThreadState(ThreadStatus::kClosePending);
                    if (::CloseHandle(hThread)) {
                        this->SetThreadState(ThreadStatus::kStopped);

                        hThread = NULL;
                        nThreadId = 0;
                        nStatus = ThreadStatus::kUnStarted;

                        bStopped = true;
                    }
                }
            }
            if (!bStopped)
                this->SetThreadState(oldStatus);
        }
    }

    jmLog.info("ThreadBase<T>::Join() Over.");
    return this->GetThreadFullState();
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Abort(uint32_t uWaitTime /* = Timeout::kTerminateWaitTime */,
                                                             uint32_t uExitCode /* = 0 */)
{
    jmLog.info("ThreadBase<T>::Abort() Enter.");
    if (IsValid()) {
        thread_status_t oldStatus = this->GetThreadState();
        if (IsAliveOrRunning()) {
            this->SetThreadState(ThreadStatus::kAbortPending);
            if (::WaitForSingleObject(hThread, uWaitTime) != WAIT_OBJECT_0) {
                if (::TerminateThread(hThread, uExitCode)) {
                    this->SetThreadState(ThreadStatus::kAborted);
                }
                else {
                    jmLog.info("ThreadBase<T>::Abort(): TerminateThread() failed. oldStatus = 0x%08X, nowStatus = 0x%08X.",
                           oldStatus, this->GetThreadState());
                    this->SetThreadState(oldStatus);
                }
            }
        }
        oldStatus = this->GetThreadState();
        this->SetThreadState(ThreadStatus::kStopPending);
        if (IsValid()) {
            this->SetThreadState(ThreadStatus::kClosePending);
            if (::CloseHandle(hThread)) {
                this->SetThreadState(ThreadStatus::kStopped);
            }
            else {
                jmLog.info("ThreadBase<T>::Abort(): CloseHandle() failed. oldStatus = 0x%08X, nowStatus = 0x%08X.",
                           oldStatus, this->GetThreadState());
                this->SetThreadState(oldStatus);
            }
        }
        hThread = NULL;
        nThreadId = 0;
        nStatus = ThreadStatus::kUnStarted;
    }
    jmLog.info("ThreadBase<T>::Abort() Over.");
    return this->GetThreadState();
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Interrupt(uint32_t uWaitTime /* = Timeout::kTerminateWaitTime */,
                                                                 uint32_t uExitCode /* = 0 */)
{
    return this->Abort(uWaitTime, uExitCode);
}

template <class T>
typename ThreadBase<T>::thread_status_t ThreadBase<T>::Terminate(uint32_t uWaitTime /* = Timeout::kTerminateWaitTime */,
                                                                 uint32_t uExitCode /* = 0 */)
{
    return this->Abort(uWaitTime, uExitCode);
}

/* static */
template <class T>
void ThreadBase<T>::Sleep(uint32_t uMilliSecs)
{
    ::Sleep(uMilliSecs);
}

/**
 * the Thread class
 */
class Thread : public ThreadBase<Thread>
{
public:
    Thread(void) : ThreadBase<Thread>() { };
    Thread(thread_proc_t thread_proc) { pThreadProc = thread_proc; }
    ~Thread(void) { };

private:
    explicit Thread(const Thread &) { }
    void operator = (const Thread &) { }
};

#else

/**
 * the Thread base class
 */
template <class T>
class ThreadBase {
public:
    ThreadBase(void);
    ThreadBase(thread_proc_t thread_proc) { pThreadProc = thread_proc; }
    ~ThreadBase(void);

private:
    explicit ThreadBase(const ThreadBase &) { }
    void operator = (const ThreadBase &) { }

protected:
    thread_proc_t   pThreadProc;
};

template <class T>
ThreadBase<T>::ThreadBase(void)
: pThreadProc(NULL)
{
}

template <class T>
ThreadBase<T>::~ThreadBase(void)
{
}

/**
 * the Thread class
 */
class Thread : public ThreadBase<Thread>
{
public:
    Thread(void) : ThreadBase<Thread>() { };
    Thread(thread_proc_t thread_proc) { pThreadProc = thread_proc; }
    ~Thread(void) { };

private:
    explicit Thread(const Thread &) { }
    void operator = (const Thread &) { }
};

#endif  /* JIMI_IS_WINDOWS */

NS_JIMI_SYSTEM_END

NS_JIMI_END

#endif  /* _JIMI_SYSTEM_THREAD_H_ */
