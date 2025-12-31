#ifndef WAITHANDLE_H_INCLUDED
#define WAITHANDLE_H_INCLUDED

class waithandle
{
    public:
        void wait()
        {
            std::unique_lock l(mutex);
            condition.wait(l);
        }

        template <typename Rep, typename Period>
        std::cv_status wait_for(std::chrono::duration<Rep, Period> dur)
        {
            std::unique_lock l(mutex);
            return condition.wait_for(l, dur);
        }

        void notify_one() noexcept
        {
            condition.notify_one();
        }

        void notify_all() noexcept
        {
            condition.notify_all();
        }

    private:
        std::atomic_bool signal;
        std::condition_variable condition;
        std::mutex mutex;
};

#endif // WAITHANDLE_H_INCLUDED
