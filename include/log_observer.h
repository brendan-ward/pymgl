#pragma once

#include <iostream>
#include <ostream>

#include <mbgl/util/event.hpp>
#include <mbgl/util/logging.hpp>

namespace mgl_wrapper {

class LogObserver : public mbgl::Log::Observer {

    bool onRecord(mbgl::EventSeverity severity,
                  mbgl::Event event,
                  int64_t code,
                  const std::string &msg) {

        if (event == mbgl::Event::ParseStyle && severity == mbgl::EventSeverity::Warning) {
            std::cerr << "Error parsing style: " << msg << std::endl;
        }

        // otherwise only log out errors
        else if (severity == mbgl::EventSeverity::Error) {
            std::cerr << "Error: " << msg << std::endl;
        }

        return true;
    }
};

} // namespace mgl_wrapper