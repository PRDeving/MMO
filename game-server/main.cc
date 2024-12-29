#include <iostream>
#include <unordered_map>

#include "networking/socket.hpp"
#include "networking/impl/ENet.hpp"
#include "messages.hpp"
#include "persistence/redisImpl.hpp"

int main() {
    Persistence::RedisImpl redis;
    Networking::Socket<Networking::Impl::ENet> socket;
    
    try {
        redis.Connect("127.0.0.1", 6379);
        socket.Serve(1234);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    std::unordered_map<std::string, Networking::Handle> clients;

    bool running = true;
    socket.Listen([&running, &socket, &redis, &clients](const Networking::EVENT_TYPE event, const Networking::Handle peer, const char *buffer) {
        switch (event) {
            case Networking::EVENT_TYPE::CONNECT:
                std::cout << "Client " << peer << " connected." << std::endl;
                socket.Send(peer, "Hello client!");
                break;
            case Networking::EVENT_TYPE::DISCONNECT:
                std::cout << "Client " << peer << " disconnected." << std::endl;
                break;
            case Networking::EVENT_TYPE::RECEIVE:{
                auto type = (Networking::Messages::MESSAGE_TYPE)(*buffer);
                switch(type) {
                    case Networking::Messages::MESSAGE_TYPE::HANDSHAKE: {
                        auto handshake = (Networking::Messages::Handshake*)buffer;
                        auto user = redis.Get(handshake->token);
                        if (user.empty()) {
                            std::cout << "Client " << peer << " failed to handshake." << std::endl;
                            socket.Kick(peer);
                            break;
                        }
                        clients[user] = peer;
                        std::cout << "Client " << user << " logged in with handle " << peer << std::endl;
                        break;
                    }
                }
                running = false;
                break;
            }
        }
    }, &running);
    
    return 0;
}