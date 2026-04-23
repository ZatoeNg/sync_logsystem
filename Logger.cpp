//
// Created by 33722 on 2026/4/20.
//
#include "Logger.h"

void LogQueue::push(const std::string &msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.emplace(msg);
    if (!_queue.empty())_cond_var.notify_one();
}

bool LogQueue::pop(std::string &msg) {
    std::unique_lock<std::mutex> lock(_mutex);
    _cond_var.wait(lock, [this]() {
        return !_queue.empty() || _is_shutdown;
    });

    if (_is_shutdown && _queue.empty()) return false;

    msg = _queue.front();
    _queue.pop();
    return true;
}

void LogQueue::shutdown() {
    std::lock_guard<std::mutex> lock(_mutex);
    _is_shutdown = true;
    _cond_var.notify_all();
}

Logger::Logger(const std::string &filename) : _log_file(filename,
                                                        std::ios::out | std::ios::app), _exit_flag(false) {
    if (!_log_file.is_open()) throw std::runtime_error("Faile to open log file");

    _worker_thread = std::thread([this]() {
        std::string msg;
        while (_log_queue.pop(msg)) {
            _log_file << msg << std::endl;
        }
    });
}

Logger::~Logger() {
    _exit_flag = true;
    _log_queue.shutdown();
    if (_worker_thread.joinable()) _worker_thread.join();

    if (_log_file.is_open()) _log_file.close();
}
