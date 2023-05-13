#pragma once

#include <iostream>
#include <memory>
#include <string>

class Logger {
   public:
    Logger(const std::string &name) : _name(name) {}

    template <typename... Ts>
    inline void info(const Ts &...loggedContent) const {
        log(loggedContent...);
    }

    template <typename... Ts>
    inline void warning(const Ts &...loggedContent) const {
        log(loggedContent...);
    }

    template <typename... Ts>
    inline void severe(const Ts &...loggedContent) const {
        log(loggedContent...);
    }

    static inline void setStream(std::ostream &stream) { _stream = &stream; }

    static inline std::ostream &getStream() { return *_stream; }

   private:
    const std::string _name;

    static std::ostream *_stream;

    template <typename... Ts>
    inline void log(const Ts &...loggedContent) const {
        if (_stream == nullptr) {
            return;
        }

        *_stream << "[" << _name << "]:";
        ((*_stream << " " << loggedContent), ...) << std::endl;
    }
};
