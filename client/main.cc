#include <iostream>
#include <string.h>

#include "networking/socket.hpp"
#include "networking/impl/ENet.hpp"
#include "messages.hpp"

int main() {
    Networking::Socket<Networking::Impl::ENet> socket;

    try {
        socket.Connect("localhost", 1234);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    bool running = true;
    socket.Listen([&running, &socket](const Networking::EVENT_TYPE event, const Networking::Handle peer, const char *buffer) {
        switch (event) {
            case Networking::EVENT_TYPE::CONNECT: {
                std::cout << "Connected." << std::endl;
                
                Networking::Messages::Handshake handshake;
                strcpy(handshake.token, "123456789");
                socket.Send(peer, Networking::Pack(handshake));

                break;
            } case Networking::EVENT_TYPE::DISCONNECT:
                std::cout << "Disonnected." << std::endl;
                running = false;
                break;
            case Networking::EVENT_TYPE::RECEIVE:
                std::cout << "Received " << buffer << " from server" << std::endl;
                break;
        }

    }, &running);

    return 0;
}  