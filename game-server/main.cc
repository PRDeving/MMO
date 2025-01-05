#include <unordered_map>
#include <cstring>

#include "networking/socket.hpp"
#include "networking/impl/ENet.hpp"
#include "persistence/redisImpl.hpp"
#include "tqueue/spmc_queue.hpp"
#include "messages.hpp"

#include "message_handlers/handshake.hpp"
#include "message_handlers/player_input.hpp"

#include "game/game.hpp"

#define SERVER_TICKRATE 1000/1

int main() {
    try {
        Persistence::RedisImpl redis;
        redis.Connect("127.0.0.1", 6379);

        Networking::Socket::Server<Networking::Impl::ENet> socket(1234);
        std::unordered_map<std::string, Networking::Handle> clients;

        struct Message {
            Networking::Handle peer;
            std::string data;
        };
        SPMCQueue<Message> message_buffer(10);
        
        bool running = true;
        socket.Listen([&](Networking::Socket::Event event, Networking::Socket::StopToken *stopToken) {
            switch (event->type) {
                case Networking::EVENT_TYPE::CONNECT:
                    printf("Client %u connected.\n", event->peer);
                    break;
                case Networking::EVENT_TYPE::DISCONNECT:
                    printf("Client %u disconnected.\n", event->peer);
                    stopToken->store(true);
                    running = false;
                    break;
                case Networking::EVENT_TYPE::RECEIVE:{
                    if (event->data.empty()) break;
                    Message message = { event->peer, std::move(event->data) };
                    if (!message_buffer.Push(message)) {
                        printf("Message buffer full.\n");
                    }   
                    break;
                }
            }
        });

        auto game = Game::Game();
        auto last = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto deltatime = 0;
        while (running) {
            auto now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            deltatime = now - last;

            auto message = message_buffer.Pop();
            if (message) {
                auto type = *(Networking::Messages::MESSAGE_TYPE*)(message->data.c_str()); 
                switch (type) {
                    case Networking::Messages::MESSAGE_TYPE::HANDSHAKE:
                        Handlers::Handshake(&socket, &redis, clients, message->peer, *(Networking::Messages::Handshake*)message->data.c_str());
                        break;
                    case Networking::Messages::MESSAGE_TYPE::PLAYER_INPUT:
                        Handlers::PlayerInput(socket, game, message->peer, *(Networking::Messages::PlayerInput*)message->data.c_str());
                        break;
                }
            }
            
            if (deltatime >= SERVER_TICKRATE) {
                printf("Game Tick.\n");
                game.update();
                last = std::move(now);
            }
        }

    } catch (std::runtime_error &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    }

    return 0;
}