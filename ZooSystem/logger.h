#pragma once
#include <iostream>
#include <mutex>

class Logger {
private:
    Logger() = default;
    ~Logger() = default;
    std::mutex mtx;
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[LOG]: " << message << std::endl;
    }
};
