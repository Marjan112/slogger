#pragma once

#include <cstdio>
#include <chrono>
#include <string>
#include <mutex>

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
        log(DEBUG, fmt_str, args...);
    }

    template<typename... _Args>
    static void Info(const char* fmt_str, _Args&&... args) {
        log(INFO, fmt_str, args...);
    }

    template<typename... _Args>
    static void Warning(const char* fmt_str, _Args&&... args) {
        log(WARNING, fmt_str, args...);
    }

    template<typename... _Args>
    static void Error(const char* fmt_str, _Args&&... args) {
        log(ERROR, fmt_str, args...);
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
    static void log(LogLevel log_level, const char* fmt_str, _Args&&... args) {
        Slogger& inst = self();
        std::lock_guard<std::mutex> lock_guard(inst.mutex);
        time_t raw = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string str_time = ctime(&raw);
        str_time.pop_back();
        switch(log_level) {
            case DEBUG: {
                printf("[%s DEBUG] ", str_time.c_str());
                printf(fmt_str, args...);
                printf("\n");

                if(inst.file != nullptr) {
                    fprintf(inst.file, "[%s DEBUG] ", str_time.c_str());
                    fprintf(inst.file, fmt_str, args...);
                    fprintf(inst.file, "\n");
                }
                break;
            }
            case INFO: {
                printf("[%s INFO] ", str_time.c_str());
                printf(fmt_str, args...);
                printf("\n");

                if(inst.file != nullptr) {
                    fprintf(inst.file, "[%s INFO] ", str_time.c_str());
                    fprintf(inst.file, fmt_str, args...);
                    fprintf(inst.file, "\n");
                }
                break;
            }
            case WARNING: {
                printf("[%s WARNING] ", str_time.c_str());
                printf(fmt_str, args...);
                printf("\n");

                if(inst.file != nullptr) {
                    fprintf(inst.file, "[%s WARNING] ", str_time.c_str());
                    fprintf(inst.file, fmt_str, args...);
                    fprintf(inst.file, "\n");
                }
                break;
            }
            case ERROR: {
                printf("[%s ERROR] ", str_time.c_str());
                printf(fmt_str, args...);
                printf("\n");

                if(inst.file != nullptr) {
                    fprintf(inst.file, "[%s ERROR] ", str_time.c_str());
                    fprintf(inst.file, fmt_str, args...);
                    fprintf(inst.file, "\n");
                }
                break;
            }
        }
    }

    ~Slogger() {
        if(file != nullptr) {
            fclose(file);
        }
    }

    FILE* file;
    std::mutex mutex;
};