#pragma once
#include <string.h>
#include <chrono>
#include "game/game.hpp"

namespace Networking::Messages {
    typedef char Token[256];
    typedef char User[256];

    enum MESSAGE_TYPE {
        NONE,
        HANDSHAKE,
        SYNC,

        // Game messages
        PLAYER_INPUT,
    };

    #pragma pack(push, 1)
    typedef struct m_handshake {
        m_handshake() {}
        m_handshake(const std::string &t) {
            strcpy(token, t.c_str());
        }
        MESSAGE_TYPE type = MESSAGE_TYPE::HANDSHAKE;
        Token token;
    } Handshake;
    #pragma pack(pop)

    #pragma pack(push, 1)
    typedef struct m_sync {
        m_sync(const std::string &u) {
            strcpy(user, u.c_str());
        }
        MESSAGE_TYPE type = MESSAGE_TYPE::SYNC;
        User user;
    } Sync;
    #pragma pack(pop)


    #pragma pack(push, 1)
    typedef struct m_player_input {
        m_player_input() {}
        m_player_input(Game::Player p, unsigned int k) : player(p), keys(k) {
            created_at = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        }
        MESSAGE_TYPE type = MESSAGE_TYPE::PLAYER_INPUT;
        Game::Player player;
        unsigned int keys;
        time_t created_at;
    } PlayerInput;
    #pragma pack(pop)
}
