#pragma once
#include <enet/enet.h>
#include <string>
#include <map>
#include "../socket.hpp"

namespace Networking {
    namespace Impl {
        class ENet {
        public:
            ENet();
            ~ENet();
            void Serve(const int port);
            void Connect(const std::string &hostname, const int port);
            void Send(const Handle handle, const std::string &buffer);
            void Broadcast(const std::string &buffer);
            void Kick(const Handle handle);
            void Listen(Callback &callback, const bool *running);
        private:
            ENetHost *host;
            ENetPeer *peer;
            std::map<Handle, ENetPeer*> peers;
        };
    }
}