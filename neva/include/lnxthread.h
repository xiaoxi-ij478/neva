#ifndef LNXTHREAD_H_INCLUDED
#define LNXTHREAD_H_INCLUDED

#include "waithandle.h"
#include "timer.h"
#include "message_queue.h"

class CLnxThread
{
        using timerflag_t = unsigned long;

        struct timerparam {
            timerparam(timer &&t): t(std::forward<timer>(t)) {}
            timer t;
            std::mutex mutex;
        };

    public:
        CLnxThread();
        virtual ~CLnxThread();

        void CreateThread(bool no_need_send_msg_l);
        void StartThread();
        void StopThread();

        template <typename T>
        void PostThreadMessage(std::unique_ptr<T> &&message)
        {
            return msg_queue.put(std::forward<std::unique_ptr<T>>(message));
        }

        template <typename Rep, typename Period>
        void SafeExitThread(std::chrono::duration<Rep, Period> wait_period)
        {
            if (StopThread() && !doing_upgrade)
                thread_end_wait_handle.wait_for(wait_period);
        }
    protected:
        void SetClassName(const std::string &name);

        virtual bool InitInstance() = 0;
        virtual bool Run();
        virtual void DispathMessage() = 0;
        virtual bool OnTimerEnter(timerflag_t tflag);
        virtual void OnTimerLeave(timerflag_t tflag);
        virtual void OnTimer(timerflag_t tflag) = 0;
        virtual bool ExitInstance() = 0;
        virtual bool KillTimer(timerflag_t tflag);

        template <typename Rep, typename Period>
        void SetTimer(
            timerflag_t tflag,
            std::chrono::duration<Rep, Period> interval
        )
        {
            std::lock_guard l(timer_mutex);
            // *INDENT-OFF*
            timerflag_t new_timerid = *std::adjacent_find(
                timerids.cbegin(),
                timerids.cend(),
                [](const timerflag_t i, const timerflag_t j) {
                    return i != j - 1;
                }
            ) + 1;
            // *INDENT-ON*
            timerids.insert(new_timerid);
            timers.insert(
                std::pair(
                    tflag,
                    timerparam(
                        timer(
                            interval,
                            interval,
                            std::mem_fn(&CLnxThread::_OnTimerEntry),
                            this,
                            tflag
                        )
                    )
                )
            );
        }
    private:
        void KillAllTimer();
        void LnxEndThread();
        void _LnxThreadEntry();
        void _OnTimerEntry(timerflag_t tflag);

    public:
        bool doing_upgrade;
        std::thread thread_obj;

    protected:
        message_queue msg_queue;

    private:
        bool thread_inited;
        bool thread_running;
        waithandle thread_end_wait_handle;
        std::string classname;
        bool no_need_send_msg;
        waithandle create_thread_wait_handle;
        waithandle start_thread_wait_handle;
        std::set<timerflag_t> timerids;
        std::unordered_map<timerflag_t, struct timerparam> timers;
        std::mutex timer_mutex;
};

#endif // LNXTHREAD_H_INCLUDED
