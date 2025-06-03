#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>

class SimpleLogger {
  private:
    std::unique_ptr<std::ofstream> file_stream_;
    mutable std::mutex mutex_;

    std::string get_timestamp() const
    {
        auto now = std::chrono::system_clock::now();
        auto time_tt = std::chrono::system_clock::to_time_t(now);
        auto ms =
            std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_tt), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

  public:
    static SimpleLogger& instance()
    {
        static SimpleLogger logger;
        return logger;
    }

    void init(const std::string& filename = "../zpr25l.log")
    {
        std::lock_guard<std::mutex> lock(mutex_);
        file_stream_ = std::make_unique<std::ofstream>(filename, std::ios::app);
        if (!file_stream_->is_open()) {
            std::cerr << "Warning: Could not open log file: " << filename << std::endl;
        }
    }

    template <typename... Args>
    void log(const std::string& level, Args&&... args)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string timestamp = get_timestamp();

        std::stringstream ss;
        ss << "[" << timestamp << "] [" << level << "] ";
        (ss << ... << args);
        std::string message = ss.str();

        std::cout << message << std::endl;

        if (file_stream_ && file_stream_->is_open()) {
            *file_stream_ << message << std::endl;
            file_stream_->flush();
        }
    }
};

inline void init_logging(const std::string& filename = "../zpr25l.log")
{
    SimpleLogger::instance().init(filename);
}

#define LOG_TRACE   SimpleLogger::instance().log("TRACE",
#define LOG_DEBUG   SimpleLogger::instance().log("DEBUG",
#define LOG_INFO    SimpleLogger::instance().log("INFO",
#define LOG_WARNING SimpleLogger::instance().log("WARNING",
#define LOG_ERROR   SimpleLogger::instance().log("ERROR",
#define LOG_FATAL   SimpleLogger::instance().log("FATAL",