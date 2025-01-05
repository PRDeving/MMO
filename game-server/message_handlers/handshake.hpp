#pragma once
#include "networking/networking.hpp"
#include "messages.hpp"

namespace Handlers {
    void Handshake(auto *socket, auto *redis, auto &clients, const Networking::Handle peer, Networking::Messages::Handshake &message) {
        auto handshake = std::move(message);
        auto user = redis->Get(handshake.token);

        if (user.empty()) {
            printf("Client %u failed to handshake.\n", peer);
            socket->Kick(peer);
        }

        clients[user] = peer;
        printf("Client %u logged in as %s.\n", peer, user.c_str());

        Networking::Messages::Sync sync(user);
        socket->Send(peer, sync);
    }
}