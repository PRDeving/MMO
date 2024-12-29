#pragma once
#include <string>
#include <functional>

namespace Networking {
    enum EVENT_TYPE {
        CONNECT = 1,
        DISCONNECT = 2,
        RECEIVE = 3
    };

    typedef int Handle;
    typedef const std::function<void(
        const EVENT_TYPE,
        const Handle peer,
        const char* data
    )> Callback;

    template<typename T>
    std::string Pack(T &message) {
        return std::string(reinterpret_cast<char*>(&message), sizeof(message));
    }
}