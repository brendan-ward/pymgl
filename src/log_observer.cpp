#include <mbgl/util/enum.hpp>
#include <mbgl/util/logging.hpp>

#include "log_observer.h"

namespace mgl_wrapper {

Log::Message::Message(mbgl::EventSeverity severity_,
                      mbgl::Event event_,
                      int64_t code_,
                      std::string msg_)
    : severity(severity_)
    , event(event_)
    , code(code_)
    , msg(std::move(msg_)) {}

bool Log::Message::matches(const Message &rhs, bool substring) const {
    return severity == rhs.severity && event == rhs.event && code == rhs.code
           && (substring ? msg.find(rhs.msg) != std::string::npos : msg == rhs.msg);
}

Log::Observer::Observer(Log *log_)
    : log(log_) {}

Log::Observer::~Observer() {
    if (log) {
        log->observer = nullptr;
    }
}

bool Log::Observer::onRecord(mbgl::EventSeverity severity,
                             mbgl::Event event,
                             int64_t code,
                             const std::string &msg) {

    std::lock_guard<std::mutex> lock(messagesMutex);

    if (event == mbgl::Event::ParseStyle && severity == mbgl::EventSeverity::Warning) {
        messages.emplace_back(severity, event, code, msg);
        std::cerr << "Error parsing style: " << msg << std::endl;
    }

    // otherwise only log out errors
    else if (severity == mbgl::EventSeverity::Error) {
        messages.emplace_back(severity, event, code, msg);
        std::cerr << "Error: " << msg << std::endl;
    }

    return true;
}

bool Log::Observer::empty() const {
    std::lock_guard<std::mutex> lock(messagesMutex);

    return messages.empty();
}

size_t Log::Observer::count(const Message &message, bool substring) const {
    std::lock_guard<std::mutex> lock(messagesMutex);

    size_t message_count = 0;
    for (const auto &msg : messages) {
        if (!msg.checked && msg.matches(message, substring)) {
            message_count++;
            msg.checked = true;
        }
    }
    return message_count;
}

Log::Log()
    : observer(new LogObserver(this)) {
    mbgl::Log::setObserver(std::unique_ptr<mbgl::Log::Observer>(observer));
    mbgl::Log::useLogThread(false);
}

bool Log::empty() const { return observer ? observer->empty() : true; }

size_t Log::count(const Log::Message &message, bool substring) const {
    return observer ? observer->count(message, substring) : 0;
}

size_t Log::uncheckedCount() const { return observer ? observer->uncheckedCount() : 0; }

Log::~Log() {
    if (observer) {
        mbgl::Log::removeObserver();
    }
}

std::vector<Log::Message> LogObserver::unchecked() const {
    std::lock_guard<std::mutex> lock(messagesMutex);

    std::vector<Message> unchecked_messages;
    for (const auto &msg : messages) {
        if (!msg.checked) {
            unchecked_messages.push_back(msg);
            msg.checked = true;
        }
    }
    return unchecked_messages;
}

size_t LogObserver::uncheckedCount() const {
    std::lock_guard<std::mutex> lock(messagesMutex);

    size_t result = 0;
    for (const auto &msg : messages) {
        if (!msg.checked) {
            result++;
        }
    }
    return result;
}

::std::ostream &operator<<(::std::ostream &os, const std::vector<Log::Message> &messages) {
    for (const auto &message : messages) {
        os << "- " << message;
    }
    return os;
}

::std::ostream &operator<<(::std::ostream &os, const Log::Message &message) {
    os << R"([")" << mbgl::Enum<mbgl::EventSeverity>::toString(message.severity) << R"(", ")";
    os << mbgl::Enum<mbgl::Event>::toString(message.event) << R"(")";
    os << ", " << message.code;
    os << R"(, ")" << message.msg << R"(")";
    return os << "]" << std::endl;
}

} // namespace mgl_wrapper
