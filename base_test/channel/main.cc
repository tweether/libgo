/************************************************
 * libgo sample3 channel
************************************************
 * 在编写比较复杂的网络程序时，经常需要在多个协程
 * 间传递数据，此时就需要用到channel。
 *
************************************************/
#include <libgo/libgo.h>
#include <stdio.h>
#include <atomic>
#include <vector>
#include <sstream>
#include <iomanip>
#include <signal.h>

#if defined(_WIN32)
#define LOG(message, ...) fprintf(stdout, "[%s] " message "\n", get_time().c_str(), __VA_ARGS__)
#else
#define LOG(message, args...) fprintf(stdout, "[%s] " message "\n", get_time().c_str(), ##args)
#endif

std::string get_time()
{
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    // oss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S"); // HH:MM:SS
    oss << std::put_time(&bt, "%F %T"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

std::atomic_bool is_running{ true };
// bool is_running = true;

void signal_handler(int sig) {
    if (sig == SIGINT) {
        LOG("Catch signal SIGINT.");
        is_running = false;
    }
}


int main(int argc, char** argv)
{
    signal(SIGINT, signal_handler);

    // ::co::CoroutineOptions::getInstance().debug |= co::dbg_channel;
    // ::co::CoroutineOptions::getInstance().debug |= co::dbg_suspend;

    co_chan<int> channel;

    auto faster_consumer = [=] (const std::string &name) {
        while (is_running) {
            int pop_num = 0;
            auto success = channel.TimedPop(pop_num, std::chrono::milliseconds(1));
            if (success) {
                if (0 == pop_num) {
                    LOG("%s Pop error.", name.c_str());
                } else {
                    LOG("%s Pop success: %d", name.c_str(), pop_num);
                }
            } else {
                // LOG("A Pop unsuccess");
            }
        }
    };

    auto consumer = [=] (const std::string &name) {
        while (is_running) {
            int pop_num = 0;
            auto success = channel.TimedPop(pop_num, std::chrono::milliseconds(20));
            if (success) {
                if (0 == pop_num) {
                    LOG("%s Pop error.", name.c_str());
                } else {
                    LOG("%s Pop success: %d", name.c_str(), pop_num);
                }

                auto timeout = std::chrono::milliseconds(20);
                if (co::Processer::IsCoroutine()) {
                    co::Processer::Suspend(timeout);
                    co::Processer::StaticCoYield();
                } else {
                    std::this_thread::sleep_for(timeout);
                }
            } else {
                // LOG("A Pop unsuccess");
            }
        }
    };

    auto producer = [=] {
        int i = 0;
        while (is_running) {
            auto timeout = std::chrono::seconds(1);
            if (co::Processer::IsCoroutine()) {
                co::Processer::Suspend(timeout);
                co::Processer::StaticCoYield();
            } else {
                std::this_thread::sleep_for(timeout);
            }
            i++;
            channel << i;
            // channel << ++i;
        }
    };

    // 多协程运行
    go std::bind(consumer, "A");
    go std::bind(consumer, "B");
    go std::bind(consumer, "C");
    go producer;

    std::vector<std::thread> thread_list;
    // 多线程运行
    // thread_list.emplace_back(std::bind(consumer, "A"));
    // thread_list.emplace_back(std::bind(consumer, "B"));
    // thread_list.emplace_back(std::bind(consumer, "C"));
    // thread_list.emplace_back(producer);

    auto sched_thread = std::thread([] () {
        co_sched.Start();       // 单processer
        // co_sched.Start(4);      // 多processer
        LOG("Complete coroutine schedule stoped.");
    });

    while (is_running) { }

    channel.Close();
    for (auto &t : thread_list) {
        if (t.joinable()) {
            t.join();
        }
    }
    LOG("Try to stop coroutine schedule.");
    co_sched.Stop();
    sched_thread.join();
    return 0;
}

