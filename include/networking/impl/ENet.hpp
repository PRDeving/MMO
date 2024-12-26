#pragma once
#include <enet/enet.h>
#include <string>
#include <map>
#include "../networking.hpp"

namespace Networking {
    class ENetImpl : public NetBase {
    public:
        ENetImpl();
        ~ENetImpl();
        void Serve(const int port) override;
        void Connect(const std::string &hostname, const int port) override;
        void Send(const Handle handle, const std::string &buffer) override;
        void Broadcast(const std::string &buffer) override;
        void Listen(const std::function<void(const EVENT_TYPE, const Handle peer, const std::string &)> &callback, const bool *running) override;
    private:
        ENetHost *host;
        ENetPeer *peer;
        std::map<Handle, ENetPeer*> peers;
    };
}