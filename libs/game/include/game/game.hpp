#pragma once

#include <memory>
#include <entt/entt.hpp>
#include "tqueue/spmc_queue.hpp"
#include <queue>

namespace Game {
    typedef uint32_t EntityID;
    typedef uint32_t Player;
    namespace Message {
        struct Keyboard {
            unsigned int keys;
        };

        struct Mouse {
            char buttons;
            int x, y;
        };

        struct Input {
            Keyboard keyboard;
            Mouse mouse;
            time_t created_at;
        };

        struct InputEvent {
            Player player;
            Input input;
        };
    }

    class Game {
        public:
            Game();
            void update();
            std::queue<Message::InputEvent> input_buffer;

        private:
            entt::registry registry;
            time_t last_tick = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    };
}