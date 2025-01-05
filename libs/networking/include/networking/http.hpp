#pragma once
#include "networking.hpp"

namespace Networking::HTTP {
    typedef const std::function<void(const std::string &data)> Callback;
}