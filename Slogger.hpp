#pragma once

#include <cstdio>
#include <chrono>
#include <string>
#include <mutex>
#include <cstdarg>
#include <cassert>

class Slogger {
public:
    static bool EnableFileLogging(const std::string& filename) {
        Slogger& inst = self();
        std::lock_guard<std::mutex> lock_guard(inst.mutex);
        inst.file = fopen(filename.c_str(), "a");
        
        return inst.file != nullptr;
    }

    template<typename... _Args>
    static void Debug(const char* fmt_str, _Args&&... args) {
        self().log(DEBUG, fmt_str, args...);
    }

    template<typename... _Args>
    static void Info(const char* fmt_str, _Args&&... args) {
        self().log(INFO, fmt_str, args...);
    }

    template<typename... _Args>
    static void Warning(const char* fmt_str, _Args&&... args) {
        self().log(WARNING, fmt_str, args...);
    }

    template<typename... _Args>
    static void Error(const char* fmt_str, _Args&&... args) {
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
    void log(LogLevel log_level, const char* fmt_str, _Args&&... args) {
        std::lock_guard<std::mutex> lock_guard(mutex);
        time_t raw = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string str_time = ctime(&raw);
        str_time.pop_back();
        switch(log_level) {
            case DEBUG: {
                printf("[%s DEBUG] ", str_time.c_str());
                log_format(stdout, fmt_str, args...);
                printf("\n");

                if(file != nullptr) {
                    fprintf(file, "[%s DEBUG] ", str_time.c_str());
                    log_format(file, fmt_str, args...);
                    fprintf(file, "\n");
                }
                break;
            }
            case INFO: {
                printf("[%s INFO] ", str_time.c_str());
                log_format(stdout, fmt_str, args...);
                printf("\n");

                if(file != nullptr) {
                    fprintf(file, "[%s INFO] ", str_time.c_str());
                    log_format(file, fmt_str, args...);
                    fprintf(file, "\n");
                }
                break;
            }
            case WARNING: {
                printf("[%s WARNING] ", str_time.c_str());
                log_format(stdout, fmt_str, args...);
                printf("\n");

                if(file != nullptr) {
                    fprintf(file, "[%s WARNING] ", str_time.c_str());
                    log_format(file, fmt_str, args...);
                    fprintf(file, "\n");
                }
                break;
            }
            case ERROR: {
                printf("[%s ERROR] ", str_time.c_str());
                log_format(stdout, fmt_str, args...);
                printf("\n");

                if(file != nullptr) {
                    fprintf(file, "[%s ERROR] ", str_time.c_str());
                    log_format(file, fmt_str, args...);
                    fprintf(file, "\n");
                }
                break;
            }
        }
    }

    void log_format(FILE* file, const char* fmt_str...) {
        assert(file != NULL);

        va_list args;
        va_start(args, fmt_str);
        vfprintf(file, fmt_str, args);
        va_end(args);
    }

    ~Slogger() {
        if(file != nullptr) {
            fclose(file);
        }
    }

    FILE* file;
    std::mutex mutex;
};