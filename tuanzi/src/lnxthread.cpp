#include "all.h"
#include "lnxthread.h"

CLnxThread::CLnxThread()
{}

CLnxThread::~CLnxThread()
{
    thread_end_wait_handle.notify_all();
}

void CLnxThread::CreateThread(bool no_need_send_msg_l)
{
    if (thread_inited)
        return;

    no_need_send_msg = no_need_send_msg_l;
    thread_inited = true;
    thread_obj = std::thread(std::mem_fn(&CLnxThread::_LnxThreadEntry), this);
    create_thread_wait_handle.wait();
}

void CLnxThread::StartThread()
{
    if (no_need_send_msg)
        return;

    PostThreadMessage(
        std::make_unique<struct base_message>(
            message_type::start_thread
        )
    );
    start_thread_wait_handle.wait();
}

void CLnxThread::StopThread()
{
    thread_running = false;

    if (no_need_send_msg)
        return;

    PostThreadMessage(
        std::make_unique<struct base_message>(
            message_type::stop_thread
        )
    );
}

void CLnxThread::SetClassName(const std::string &name)
{
    classname = name;
}

bool CLnxThread::Run()
{
    bool start_process = false;

    if (no_need_send_msg)
        return true;

    while (true)
        switch (msg_queue.get_type()) {
            case message_type::stop_thread:
                return false;

            case message_type::start_thread:
                start_process = true;
                start_thread_wait_handle.notify_all();
                break;

            default:
                if (start_process)
                    DispathMessage();

                break;
        }

    return true;
}

bool CLnxThread::OnTimerEnter(timerflag_t tflag)
{
    try {
        struct timerparam &timer_param = timers.at(tflag);

        if (!timer_param.mutex.try_lock())
            return false;

        return true;

    } catch (const std::out_of_range&) {
        return false;
    }
}

void CLnxThread::OnTimerLeave(timerflag_t tflag)
{
    try {
        struct timerparam &timer_param = timers.at(tflag);
        timer_param.mutex.unlock();

    } catch (const std::out_of_range&) {}
}

bool CLnxThread::KillTimer(timerflag_t tflag)
{
    std::lock_guard l(timer_mutex);

    if (!tflag)
        return false;

    if (timers.empty())
        return true;

    timerids.erase(tflag);
    timers.erase(tflag);
    return true;
}

void CLnxThread::KillAllTimer()
{
    std::lock_guard l(timer_mutex);
    timerids.clear();
    timers.clear();
}

void CLnxThread::LnxEndThread()
{
    KillAllTimer();
    thread_running = false;
    start_thread_wait_handle.notify_all();
    thread_end_wait_handle.notify_all();
}

void CLnxThread::_LnxThreadEntry()
{
    thread_running = true;
    create_thread_wait_handle.notify_one();

    if (InitInstance())
        Run();

    else
        ExitInstance();

    LnxEndThread();
}

void CLnxThread::_OnTimerEntry(timerflag_t tflag)
{
    OnTimer(tflag);
}
