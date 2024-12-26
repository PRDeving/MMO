#include <iostream>
#include "networking/impl/ENet.hpp"

int main() {
    Networking::ENetImpl net;
    net.Connect("localhost", 1234);

    bool running = true;
    net.Listen([&running](const Networking::EVENT_TYPE event, const Networking::Handle peer, const std::string &buffer) {
        switch (event) {
            case Networking::EVENT_TYPE::CONNECT:
                std::cout << "Connected." << std::endl;
                break;
            case Networking::EVENT_TYPE::DISCONNECT:
                std::cout << "Disonnected." << std::endl;
                break;
            case Networking::EVENT_TYPE::RECEIVE:
                std::cout << "Received " << buffer << " from server" << std::endl;
                running = false;
                break;
        }

    }, &running);

    return 0;
}  