#include <thread>
#include <queue>
#include "../Slogger.hpp"

std::queue<int> data_queue;
std::mutex queue_mutex;

void producer() {
    for(int i = 0; i < 10; ++i) {
        std::lock_guard<std::mutex> lock_guard(queue_mutex);
        data_queue.push(i);
        Slogger::Debug("Produced: %d", i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void consumer() {
    for(int i = 0; i < 10; ++i) {
        std::lock_guard<std::mutex> lock_guard(queue_mutex);
        if(!data_queue.empty()) {
            int consumed = data_queue.front();
            data_queue.pop();
            Slogger::Debug("Consumed: %d", consumed);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    Slogger::EnableFileLogging("logfile.log");

    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);

    producer_thread.join();
    consumer_thread.join();
    return 0;
}