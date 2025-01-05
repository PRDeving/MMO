#pragma once
#include <thread>
#include <atomic>
#include <future>
#include "networking.hpp"

namespace Networking {

    namespace Socket {
        typedef std::atomic<bool> StopToken;
        typedef const std::function<void(
            const EVENT_TYPE,
            const Handle peer,
            const char* data,
            StopToken* listener
        )> Callback;

        struct s_event{
            EVENT_TYPE type;
            Handle peer;
            std::string data;
        };

        typedef std::unique_ptr<s_event> Event;

        class Impl {
            public:
                virtual void Serve(int port) = 0;
                virtual void Connect(const std::string &hostname, const int port) = 0;
                virtual void Send(const Handle handle, const std::string &buffer) = 0;
                virtual void Send(const std::string &buffer) = 0;
                virtual void Broadcast(const std::string &buffer) = 0;
                virtual void Kick(Handle handle) = 0;
                virtual void Listen(std::function<void(Networking::Socket::Event, std::atomic<bool>*)> callback) = 0;
        };

        // TODO: Implement a concept for this
        // I'd really like to use C++20 concepts here, but I have to fiddle with github codespaces config first to update the compiler
        template<typename T>
        class Server {
            public:
                Server(int port) {
                    impl_.Serve(port);
                }

                void Send(const Handle handle, auto &message) {
                    auto m = std::move(message);
                    std::string payload = std::string(reinterpret_cast<char*>(&m), sizeof(m));
                    impl_.Send(handle, payload);
                }

                void Broadcast(const std::string &buffer) {
                    impl_.Broadcast(buffer);
                }

                void Kick(int handle) {
                    impl_.Kick(handle);
                }

                void Listen(std::function<void(Networking::Socket::Event, std::atomic<bool>*)> callback) {
                    impl_.Listen(callback);
                }

            private:
                T impl_;
        };
    }

}