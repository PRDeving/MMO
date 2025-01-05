#pragma once
#include <enet/enet.h>
#include <string>
#include <unordered_map>
#include <atomic>
#include "../socket.hpp"
#include <thread>

namespace Networking {
    namespace Impl {
        class ENet {
        public:
            ENet();
            ~ENet();
            void Serve(const int port);
            void Connect(const std::string &hostname, const int port);
            void Send(ENetPeer *p, const std::string &buffer);
            void Send(const Handle handle, const std::string &buffer);
            void Send(const std::string &buffer);
            void Broadcast(const std::string &buffer);
            void Kick(const Handle handle);
            void Listen(std::function<void(Networking::Socket::Event, std::atomic<bool>*)> callback);
        private:
            ENetHost *host = nullptr;
            ENetPeer *peer = nullptr;
            std::unordered_map<Handle, ENetPeer*> peers;
            Networking::Socket::StopToken stop_token{false};
            std::thread listener;
        };
    }
}