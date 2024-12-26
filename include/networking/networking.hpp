#pragma once
#include <string>
#include <functional>

namespace Networking {
    typedef int Handle;

    enum EVENT_TYPE {
        CONNECT = 1,
        DISCONNECT = 2,
        RECEIVE = 3
    };

    class NetBase {
    public:
        NetBase() = default;
        virtual void Serve(const int port) = 0;
        virtual void Connect(const std::string &hostname, const int port) = 0;
        virtual void Send(const Handle handle, const std::string &buffer) = 0;
        virtual void Broadcast(const std::string &buffer) = 0;
        virtual void Listen(const std::function<void(const EVENT_TYPE, const Handle peer, const std::string &)> &callback, const bool *running) = 0;
    };
}