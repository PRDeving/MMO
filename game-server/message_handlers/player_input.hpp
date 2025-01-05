#pragma once
#include "messages.hpp"
#include "networking/networking.hpp"

namespace Handlers {
    void PlayerInput(auto &socket, auto &game, Networking::Handle peer, Networking::Messages::PlayerInput &message) {
        auto input = std::move(message);
        Game::Message::InputEvent i = {
            .player = input.player,
            .input = {
                .keyboard = { .keys = input.keys },
                .mouse = {},
                .created_at = input.created_at
            }
        };
        game.input_buffer.push(std::move(i));
        // Kick the player after receiving input so everything goes down gracefully until my lazy ass implements signal handling
        socket.Kick(peer);
    }
}