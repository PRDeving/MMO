#include <cstring>
#include <optional>
#include "networking/socket.hpp"
#include "networking/impl/ENet.hpp"
#include "networking/http.hpp"
#include "networking/impl/cUrl.hpp"
#include "messages.hpp"

#include "infra/gateway-service.hpp"
#include "infra/game-server.hpp"

#include "game/game.hpp"

int main() {

    Infra::GatewayService::Repository<Networking::Impl::cUrl> gateway("http://localhost:3000");

    auto [host, port, token] = gateway.Login("user", "password").get();
    Infra::GameServer::Repository<Networking::Impl::ENet> gameServer(host, port);

    gameServer.Listen([&](Networking::Socket::Event event, Networking::Socket::StopToken *stopToken) {
        switch (event->type) {
            case Networking::EVENT_TYPE::CONNECT: {
                printf("Connected.\n");
                gameServer.Handshake(token);
                break;
            } case Networking::EVENT_TYPE::DISCONNECT:
                printf("Disconnected.\n");
                stopToken->store(true);
                break;
            case Networking::EVENT_TYPE::RECEIVE:
                if (event->data.empty()) break;

                auto type = static_cast<Networking::Messages::MESSAGE_TYPE>(*event->data.c_str());
                switch(type) {
                    case Networking::Messages::MESSAGE_TYPE::SYNC: {
                        auto sync = reinterpret_cast<const Networking::Messages::Sync*>(event->data.c_str());
                        printf("Synced with user %s\n", sync->user);

                        // Send player input
                        auto input = Networking::Messages::PlayerInput(Game::Player(1337), (unsigned int)0x1);
                        gameServer.PlayerInput(input);
                    }
                }
                break;
        }
    });

    return 0;
}  