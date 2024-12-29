#include <enet/enet.h>
#include <stdexcept>
#include "networking/impl/ENet.hpp"
#include <iostream>

namespace Networking {
    
    Impl::ENet::ENet() {
        if(enet_initialize() != 0) {
            throw std::runtime_error("ENet: Failed to initialize.");
        }
        std::cout << "ENet: Initialized." << std::endl;
    }

    Impl::ENet::~ENet() {
        enet_host_destroy(host);
        enet_deinitialize();
        std::cout << "ENet: Deinitialized." << std::endl;
    }

    void Impl::ENet::Serve(const int port) {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;

        host = enet_host_create(&address, 32, 2, 0, 0);
        if(host == nullptr) {
            throw std::runtime_error("ENet: Failed to create host.");
        }
        std::cout << "ENet: Host created." << std::endl;
    }

    void Impl::ENet::Connect(const std::string &hostname, const int port) {
        host = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);
        if (!host) {
            throw std::runtime_error("ENet: Failed to create host.");
        }

        ENetAddress address;
        if(enet_address_set_host(&address, hostname.c_str()) != 0) {
            throw std::runtime_error("ENet: Failed to resolve hostname.");
        }
        address.port = port;

        peer = enet_host_connect(host, &address, 2, 0);
        if(peer == nullptr) {
            throw std::runtime_error("ENet: Failed to connect to peer.");
        }
        std::cout << "ENet: Connected to peer." << std::endl;
    }

    void Impl::ENet::Send(const Handle handle, const std::string &buffer) {
        ENetPacket *packet = enet_packet_create(buffer.c_str(), buffer.length(), ENET_PACKET_FLAG_RELIABLE);
        ENetPeer *peer = peers[handle];
        enet_peer_send(peer, 0, packet);
    }

    void Impl::ENet::Broadcast(const std::string &buffer) {
        ENetPacket *packet = enet_packet_create(buffer.c_str(), buffer.length(), ENET_PACKET_FLAG_RELIABLE);
        enet_host_broadcast(host, 0, packet);
    }

    void Impl::ENet::Kick(const Handle handle) {
        enet_peer_disconnect(peers[handle], 0);
    }

    void Impl::ENet::Listen(Callback &callback, const bool *running) {
        ENetEvent event;
        while (*running){
            while (enet_host_service(host, &event, 0) > 0) {

                if (event.type == ENET_EVENT_TYPE_CONNECT) {
                    event.peer->data = new Handle(event.peer->connectID);
                    peers[*(int*)event.peer->data] = event.peer;
                }

                if(event.packet) {
                    callback(static_cast<EVENT_TYPE>(event.type), *(Handle*)event.peer->data, (char*)event.packet->data);
                    enet_packet_destroy(event.packet);
                } else callback(static_cast<EVENT_TYPE>(event.type), *(Handle*)event.peer->data, "");

                if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                    peers.erase(*(int*)event.peer->data);
                    delete (int*)event.peer->data;
                }
            }
        }
    }
}