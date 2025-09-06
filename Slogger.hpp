#pragma once

#include <print>
#include <fstream>
#include <chrono>
#include <mutex>

class Slogger {
public:
    static bool EnableFileLogging(const std::string& filename) {
        Slogger& inst = self();
        std::lock_guard<std::mutex> lock_guard(inst.mutex);
        
        inst.file.open(filename);
        return !inst.file.fail();
    }

    template<typename... _Args>
    static void Debug(std::string_view fmt_str, _Args&&... args) {
        self().log(DEBUG, fmt_str, args...);
    }

    template<typename... _Args>
    static void Info(std::string_view fmt_str, _Args&&... args) {
        self().log(INFO, fmt_str, args...);
    }

    template<typename... _Args>
    static void Warning(std::string_view fmt_str, _Args&&... args) {
        self().log(WARNING, fmt_str, args...);
    }

    template<typename... _Args>
    static void Error(std::string_view fmt_str, _Args&&... args) {
        self().log(ERROR, fmt_str, args...);
    }
private:
    static Slogger& self() {
        static Slogger slogger;
        return slogger;
    }

    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    template<typename... _Args>
    void log(LogLevel log_level, std::string_view fmt_str, _Args&&... args) {
        std::lock_guard<std::mutex> lock_guard(mutex);
        auto now_seconds = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
        auto local_time = std::chrono::zoned_time {std::chrono::current_zone(), now_seconds};
        
        std::string format_args = std::vformat(fmt_str, std::make_format_args(args...));
         
        switch(log_level) {
            case DEBUG: {
                std::println("[{:%Y-%m-%d %H-%M-%S} DEBUG] {}", local_time, format_args);
                if(file.is_open()) std::println(file, "[{:%Y-%m-%d %H-%M-%S} DEBUG] {}", local_time, format_args); 
                break;
            }
            case INFO: {
                std::println("[{:%Y-%m-%d %H-%M-%S} INFO] {}", local_time, format_args);
                if(file.is_open()) std::println(file, "[{:%Y-%m-%d %H-%M-%S} INFO] {}", local_time, format_args);  
                break;
            }
            case WARNING: { 
                std::println("[{:%Y-%m-%d %H-%M-%S} WARNING] {}", local_time, format_args);
                if(file.is_open()) std::println(file, "[{:%Y-%m-%d %H-%M-%S} WARNING] {}", local_time, format_args); 
                break;
            }
            case ERROR: {
                std::println("[{:%Y-%m-%d %H-%M-%S} ERROR] {}", local_time, format_args);
                if(file.is_open()) std::println(file, "[{:%Y-%m-%d %H-%M-%S} ERROR] {}", local_time, format_args);  
                break;
            }
        }
    }

    std::ofstream file;
    std::mutex mutex;
};
