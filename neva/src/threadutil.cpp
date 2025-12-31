#include "all.h"
#include "cmdutil.h"
#include "timeutil.h"
#include "util.h"
#include "waithandle.h"
#include "global.h"
#include "miscdefs.h"
#include "mtypes.h"
#include "threadutil.h"

bool TerminateThread(pthread_t thread_id)
{
    int retval = 0;
    int msqid = 0;
    void *thread_return = nullptr;

    if ((retval = pthread_cancel(thread_id))) {
        g_logSystem.AppendText("pthread_cancel Error:%d\n", retval);
        return retval;
    }

    if ((retval = pthread_join(thread_id, &thread_return))) {
        g_logSystem.AppendText("pthread_join ret:%d error:%s", retval, strerror(errno));
        return retval;
    }

    msqid =
        msgget(
            thread_id,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
        );

    if (msqid == -1)
        return retval;

    if (msgctl(msqid, IPC_RMID, nullptr))
        g_logSystem.AppendText("msgctl error:%s", strerror(errno));

    return retval;
}

bool PostThreadMessage(
    key_t thread_key,
    long mtype,
    unsigned long arg1,
    unsigned long arg2
)
{
    int msqid =
        msgget(
            thread_key,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
        );
    g_logFile_start.AppendText(
        "::PostThreadMessage idThread = %d,Msg=%d",
        thread_key,
        mtype
    );
    return msqid >= 0 ? GPostThreadMessage(msqid, mtype, arg1, arg2) : false;
}

bool GPostThreadMessage(
    int msqid,
    long mtype,
    unsigned long arg1,
    unsigned long arg2
)
{
    struct LNXMSG msg = { mtype, arg1, arg2 };
    int ret = 0;

    if (msqid < 0) {
        rj_printf_debug("message id is null,return\n");
        return false;
    }

    ret = msgsnd(msqid, &msg, LNXMSG_MSGSZ, IPC_NOWAIT);

    if (ret == -1)
        rj_printf_debug("msgsnd Error:%s\n", strerror(errno));

    return ret != -1;
}

void StopOcx()
{
    PostThreadMessage(theApp.thread_key, STOP_OCX_MTYPE, 0, 0);
}
