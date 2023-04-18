#pragma once

#include <cstdarg>
#include <iostream>
#include <mutex>
#include <ostream>
#include <vector>

#include <mbgl/util/event.hpp>
#include <mbgl/util/logging.hpp>

namespace mgl_wrapper {

// adapted from mbgl/test/fixture_log_observer

class Log {
public:
    struct Message {
        Message() = default;
        Message(mbgl::EventSeverity severity_,
                mbgl::Event event_,
                int64_t code_,
                std::string msg_);

        bool matches(const Message &rhs, bool substring = false) const;

        const mbgl::EventSeverity severity{};
        const mbgl::Event event{};
        const int64_t code{};
        const std::string msg{};

        mutable bool checked = false;
    };

    class Observer : public mbgl::Log::Observer {
    public:
        using LogMessage = Message;

        Observer(Log *log = nullptr);
        ~Observer() override;

        // Log::Observer implementation
        bool onRecord(mbgl::EventSeverity severity,
                      mbgl::Event event,
                      int64_t code,
                      const std::string &msg) override;

        bool empty() const;
        size_t uncheckedCount() const;
        size_t count(const Message &message, bool substring = false) const;
        std::vector<Message> unchecked() const;

    private:
        Log *log;
        std::vector<Message> messages;
        mutable std::mutex messagesMutex;
    };

    Log();

    bool empty() const;
    size_t uncheckedCount() const;
    size_t count(const Message &message, bool substring = false) const;

    ~Log();

private:
    Observer *observer;
};

::std::ostream &operator<<(::std::ostream &os,
                           const std::vector<Log::Observer::LogMessage> &messages);
::std::ostream &operator<<(::std::ostream &os, const Log::Observer::LogMessage &message);

using LogObserver = Log::Observer;

// class LogObserver : public mbgl::Log::Observer {

//     bool onRecord(mbgl::EventSeverity severity,
//                   mbgl::Event event,
//                   int64_t code,
//                   const std::string &msg) {

//         if (event == mbgl::Event::ParseStyle && severity == mbgl::EventSeverity::Warning) {
//             std::cerr << "Error parsing style: " << msg << std::endl;
//         }

//         // otherwise only log out errors
//         else if (severity == mbgl::EventSeverity::Error) {
//             std::cerr << "Error: " << msg << std::endl;
//         }

//         return true;
//     }
// };

} // namespace mgl_wrapper