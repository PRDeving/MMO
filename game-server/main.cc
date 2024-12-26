#include <iostream>
#include "networking/impl/ENet.hpp"

int main() {
    Networking::ENetImpl net;
    net.Serve(1234);

    bool running = true;
    net.Listen([&net](const Networking::EVENT_TYPE event, const Networking::Handle peer, const std::string &buffer) {
        switch (event) {
            case Networking::EVENT_TYPE::CONNECT:
                std::cout << "Client " << peer << " connected." << std::endl;
                net.Send(peer, "Hello client!");
                break;
            case Networking::EVENT_TYPE::DISCONNECT:
                std::cout << "Client " << peer << " disconnected." << std::endl;
                break;
            case Networking::EVENT_TYPE::RECEIVE:
                std::cout << "Received " << buffer << " from " << peer << std::endl;
                break;
        }
    }, &running);
    
    return 0;
}  