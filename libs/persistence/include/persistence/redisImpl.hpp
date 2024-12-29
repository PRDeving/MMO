#pragma once
#include <hiredis.h>
#include <string>

namespace Persistence
{
    class RedisImpl
    {
    public:
        ~RedisImpl();
        void Connect(const std::string& host, const int port);
        void Set(const std::string& key, const std::string& value);
        std::string Get(const std::string& key);
    private:
        redisContext *context;
    };
}