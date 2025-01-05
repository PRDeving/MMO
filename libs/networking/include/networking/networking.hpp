#pragma once
#include <string>
#include <functional>

namespace Networking {
    enum EVENT_TYPE {
        CONNECT = 1,
        DISCONNECT = 2,
        RECEIVE = 3
    };

    typedef unsigned int Handle;

    std::string Pack(auto &message) {
        return std::string(reinterpret_cast<char*>(&message), sizeof(message));
    }
}