#pragma once
#include "game/game.hpp"

#define INPUT_BUFFER_SIZE 10

namespace Components {

    typedef Game::Player Player;

    struct Input {
        std::queue<Game::Message::Input> input_buffer;
        time_t last_tick = 0;
    };

    struct Position {
        float x;
        float y;
    };

    struct Velocity {
        float dx;
        float dy;
    };
}
