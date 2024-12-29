#pragma once
#include "networking.hpp"

namespace Networking {

    // TODO: Implement a concept for Impl
    // I'd really like to use C++20 concepts here, but I have to fiddle with github codespaces config first to update the compiler
    template<typename Impl>
    class Socket {
        public:
            Socket() = default;

            void Serve(int port) {
                impl_.Serve(port);
            }

            void Connect(const std::string &hostname, int port) {
                impl_.Connect(hostname, port);
            }

            void Send(int handle, const std::string &buffer) {
                impl_.Send(handle, buffer);
            }

            void Broadcast(const std::string &buffer) {
                impl_.Broadcast(buffer);
            }

            void Kick(int handle) {
                impl_.Kick(handle);
            }

            void Listen(Callback &callback, const bool *running) {
                impl_.Listen(callback, running);
            }

        private:
            Impl impl_;
    };
}