//
// Created by 33722 on 2026/4/20.
//

#ifndef SYNC_LOGSYSTEM_LOGGER_H
#define SYNC_LOGSYSTEM_LOGGER_H
#include "pch.h"

class LogQueue {
public:
    void push(const std::string &msg);

    bool pop(std::string &msg);

    void shutdown();

private:
    std::queue<std::string> _queue;
    std::mutex _mutex;
    std::condition_variable _cond_var;
    bool _is_shutdown = false;
};

enum class LogLevel {
    INFO, DEBUG, WARNING, ERROR, FATAL
};

class Logger {
public:
    Logger(const std::string &filename);

    ~Logger();

    template<typename... Args>
    void log(LogLevel level, const std::string &format, Args &&... args) {
        std::string level_string;
        switch (level) {
            case LogLevel::INFO:
                level_string = "[INFO] ";
                break;
            case LogLevel::DEBUG:
                level_string = "[DEBUG] ";
                break;
            case LogLevel::WARNING:
                level_string = "[WARNING] ";
                break;
            case LogLevel::ERROR:
                level_string = "[ERROR] ";
                break;
            case LogLevel::FATAL:
                level_string = "[FATAL] ";
                break;
        }

        _log_queue.push(level_string + format_message(format, std::forward<Args>(args)...));
    }

private:
    template<typename T>
    std::string to_string_helper(T &&arg) {
        std::ostringstream oss;
        oss << std::forward<T>(arg);
        return oss.str();
    }

    std::string get_current_time() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);

        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S",
            std::localtime(&t));

        return std::string(buffer);
    }

    template<typename... Args>
    std::string format_message(const std::string &format, Args &&... args) {
        std::vector<std::string> arg_string = {to_string_helper(std::forward<Args>(args))...};
        std::ostringstream oss;
        size_t arg_index = 0;
        size_t pos = 0;
        size_t placeholder = format.find("{}", pos);
        while (placeholder != std::string::npos) {
            oss << format.substr(pos, placeholder - pos);

            if (arg_index < arg_string.size()) oss << arg_string[arg_index++];
            else oss << "{}";

            pos = placeholder + 2;
            placeholder = format.find("{}", pos);
        }

        oss << format.substr(pos);
        while (arg_index < arg_string.size()) {
            oss << arg_string[arg_index++];
        }

        return " ["+get_current_time()+"] "+oss.str();
    }

private:
    LogQueue _log_queue;
    std::thread _worker_thread;
    std::ofstream _log_file;
    std::atomic<bool> _exit_flag;
};

#endif //SYNC_LOGSYSTEM_LOGGER_H
