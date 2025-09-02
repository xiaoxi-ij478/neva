#ifndef THREADUTIL_H_INCLUDED
#define THREADUTIL_H_INCLUDED

class waithandle;

extern int WaitForSingleObject(
    waithandle &wait_handle,
    unsigned long off_msec
);
extern void CloseHandle(waithandle *wait_handle);
extern void SetEvent(waithandle *wait_handle, bool broadcast);
extern bool TerminateThread(pthread_t thread_id);
extern bool PostThreadMessage(
    key_t thread_key,
    long mtype,
    unsigned long arg1,
    unsigned long arg2
);
extern bool GPostThreadMessage(
    int msqid,
    long mtype,
    unsigned long arg1,
    unsigned long arg2
);
extern void StopOcx();

#endif // THREADUTIL_H_INCLUDED
