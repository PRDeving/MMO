#include <enet/enet.h>
#include <stdexcept>
#include <memory>
#include "networking/impl/ENet.hpp"
#include <thread>
#include <future>

namespace Networking {
    
    Impl::ENet::ENet() {
        if(enet_initialize() != 0) {
            throw std::runtime_error("ENet: Failed to initialize.");
        }
        printf("ENet: Initialized.\n");
    }

    Impl::ENet::~ENet() {
        listener.join();
        for (auto const& [key, p] : peers) {
            delete (Handle*)p->data;
            enet_peer_disconnect(p, 0);
        }
        if (peer) enet_peer_disconnect(peer, 0);
        enet_host_destroy(host);
        enet_deinitialize();
        printf("ENet: Deinitialized.\n");
    }

    void Impl::ENet::Serve(const int port) {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;

        host = enet_host_create(&address, 32, 2, 0, 0);
        if(host == nullptr) {
            throw std::runtime_error("ENet: Failed to create host.");
        }
        printf("ENet: Host created.\n");
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
        printf("ENet: Connecting to peer.\n");
    }

    void Impl::ENet::Send(ENetPeer *p, const std::string &buffer) {
        ENetPacket *packet = enet_packet_create(buffer.c_str(), buffer.length(), ENET_PACKET_FLAG_RELIABLE);
        printf("ENet: send to %u\n", p->connectID);
        enet_peer_send(p, 0, packet);
    }

    void Impl::ENet::Send(const std::string &buffer) {
        if (peer == nullptr) throw std::runtime_error("ENet: Peer not connected.");
        Send(peer, buffer);
    }

    void Impl::ENet::Send(const Handle handle, const std::string &buffer) {
        Send(peers[handle], buffer);
    }

    void Impl::ENet::Broadcast(const std::string &buffer) {
        ENetPacket *packet = enet_packet_create(buffer.c_str(), buffer.length(), ENET_PACKET_FLAG_RELIABLE);
        enet_host_broadcast(host, 0, packet);
    }

    void Impl::ENet::Kick(const Handle handle) {
        enet_peer_disconnect(peers[handle], 0);
    }

    void Impl::ENet::Listen(std::function<void(Networking::Socket::Event, std::atomic<bool>*)> callback) {
        stop_token.store(false);
        listener = std::thread([callback, this](){
            ENetEvent event;
            printf("ENet: Listening.\n");
            while (!stop_token){
                while (enet_host_service(host, &event, 0) > 0) {

                    if (event.type == ENET_EVENT_TYPE_CONNECT) {
                        event.peer->data = new Handle(event.peer->connectID);
                        peers[*(int*)event.peer->data] = event.peer;
                    }

                    Networking::Socket::Event e = std::make_unique<Networking::Socket::s_event>(
                        Networking::Socket::s_event{    
                            static_cast<EVENT_TYPE>(event.type),
                            *(Handle*)event.peer->data,
                            event.packet ? std::string((char*)event.packet->data, event.packet->dataLength) : ""
                        }
                    );

                    callback(std::move(e), &stop_token);
                    if(event.packet) enet_packet_destroy(event.packet);

                    if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                        peers.erase(*(int*)event.peer->data);
                        delete (Handle*)event.peer->data;
                    }
                }
            }
            printf("ENet: Stopped listening.\n");
        });
    }
}