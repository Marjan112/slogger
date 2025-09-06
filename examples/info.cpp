#include <iostream>
#include <thread>
#include "../Slogger.hpp"

void thread_function(int id) {
    Slogger::Info("Thread {} is starting", id);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    Slogger::Info("Thread {} is finishing", id);
}

int main() {
    Slogger::EnableFileLogging("logfile.log");

    std::thread threads[5];

    for(int i = 0; i < sizeof(threads) / sizeof(threads[0]); ++i) {
        threads[i] = std::thread(thread_function, i);
    }

    for(int i = 0; i < sizeof(threads) / sizeof(threads[0]); ++i) {
        threads[i].join();
    }
    return 0;
}
