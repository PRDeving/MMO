
#include <hiredis.h>
#include <stdexcept>
#include "persistence/redisImpl.hpp"
#include <iostream>

namespace Persistence {
    RedisImpl::~RedisImpl() {
        redisFree(context);
        std::cout << "Hiredis: Disconnected from Redis server" << std::endl;
    }

    void RedisImpl::Connect(const std::string& host, const int port) {
        context = redisConnect(host.c_str(), port);
        if (context == NULL || context->err) {
            if (context) {
                throw std::runtime_error("Hiredis: Error: " + std::string(context->errstr));
            } else {
                throw std::runtime_error("Hiredis: Can't allocate redis context");
            }
        }
        std::cout << "Hiredis: Connected to Redis server" << std::endl;
    }

    void RedisImpl::Set(const std::string& key, const std::string& value) {
        redisReply *reply = (redisReply*)redisCommand(context, "SET %s %s", key.c_str(), value.c_str());
        if (reply == NULL) {
            throw std::runtime_error("Hiredis: SET command failed");
        }
        freeReplyObject(reply);
        std::cout << "Hiredis: SET v: " << value << " for k: " << key << std::endl;
    }

    std::string RedisImpl::Get(const std::string& key) {
        redisReply *reply = (redisReply*)redisCommand(context, "GET %s", key.c_str());
        if (reply == NULL) {
            return "";
        }

        std::string result;
        if (reply->type == REDIS_REPLY_STRING) {
            result = reply->str;
        }
        freeReplyObject(reply);
        std::cout << "Hiredis: GET k: " << key << ", v: " << result << std::endl;
        return result;
    }
}