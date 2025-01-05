#include <entt/entt.hpp>
#include <memory>
#include <queue>
#include <chrono>
#include "game/game.hpp"
#include "game/components.hpp"


void inputSystem(entt::registry &registry, auto &inputs) {
    auto view = registry.view<Components::Input, Components::Player>();
    view.each([&inputs](entt::entity e, Components::Input &input, auto &player) {
        if (inputs.find(player) == inputs.end()) return;
        
        Game::Message::Input player_inputs = inputs[player];
        input.input_buffer.push(std::move(player_inputs));

        while (input.input_buffer.size() > INPUT_BUFFER_SIZE) {
            input.input_buffer.pop();
        }
    });
}

void movementSystem(entt::registry &registry, time_t dt) {

    // Player controls applied to velocity
    auto view = registry.view<Components::Input, Components::Velocity>();
    view.each([](entt::entity e, auto &input, auto &velocity) {
        if (input.input_buffer.empty() || input.input_buffer.back().created_at <= input.last_tick) return;

        auto last_input = input.input_buffer.back();
        float dx = last_input.keyboard.keys & 0x1 ? 0.1 : 0.0f;
        float dy = last_input.keyboard.keys & 0x2 ? 0.1 : 0.0f;
        velocity.dx = dx;
        velocity.dy = dy;
        input.last_tick = last_input.created_at;
    });

    // Movement
    auto view2 = registry.view<Components::Position, const Components::Velocity>();
    view2.each([dt](entt::entity e, auto &pos, const auto &vel) {
        if (vel.dx == 0.0f && vel.dy == 0.0f) return;
        pos.x += vel.dx * dt;
        pos.y += vel.dy * dt;
        printf("Entity %u moved to %f, %f\n", (uint32_t)e, pos.x, pos.y);
    });
}

void Game::Game::update() {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // Handle player inputs
    std::unordered_map<Player, Message::Input> player_inputs;
    auto items_n = input_buffer.size();
    while (items_n--) {
        auto input = &input_buffer.front();

        if (input->input.created_at > now) {
            continue;
        } else if(
            input->input.created_at < last_tick ||
            player_inputs[input->player].created_at > input->input.created_at
        ) {
            input_buffer.pop();
            continue;
        }

        player_inputs[input->player] = std::move(input->input);
    }
    printf("Player inputs: %lu\n, total inputs: %lu\n", player_inputs.size(), input_buffer.size());

    // Execute systems
    inputSystem(registry, player_inputs);
    movementSystem(registry, now - last_tick);
    last_tick = now;
}

Game::Game::Game() {
    const auto entity = registry.create();
    registry.emplace<Components::Position>(entity, 10.f, 10.f);
    registry.emplace<Components::Velocity>(entity, 0.0f, 0.0f);
    registry.emplace<Components::Player>(entity, 1337);
    registry.emplace<Components::Input>(entity);
}