#pragma once
#include "networking/socket.hpp"
#include "game/game.hpp"

namespace Infra {
    namespace GameServer {
        template<typename T>
        class Repository {
            public:
                Repository(const std::string &host, const int port) {
                    socket_.Connect(host, port);
                }

                T &GetSocket() {
                    return socket_;
                }

                void Listen(std::function<void(Networking::Socket::Event, std::atomic<bool>*)> callback) {
                    socket_.Listen(callback);
                }
                
                void Handshake(const std::string &token) {
                    Networking::Messages::Handshake handshake(token);
                    printf("Handshaking with token %s\n", handshake.token);
                    std::string payload = Networking::Pack(handshake);
                    socket_.Send(payload);
                }

                void PlayerInput(Networking::Messages::PlayerInput &input) {
                    std::string payload = Networking::Pack(input);
                    socket_.Send(payload);
                }

            private:
                T socket_;
        };
    }
}