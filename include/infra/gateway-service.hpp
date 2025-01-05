#pragma once
#include <nlohmann/json.hpp>
#include <future>
#include "networking/http.hpp"

namespace Infra {

    namespace GatewayService {
        typedef struct {
            std::string host;
            int port;
            std::string token;
        } LoginResponse;
        using LoginResponseCallback = std::function<void(const LoginResponse&)>;
    
        template<typename T>
        class Repository {
            public:
                Repository(const std::string &host) : base_url(host) {}

                std::future<LoginResponse> Login(const std::string &username, const std::string &password) {
                    nlohmann::json j;
                    j["username"] = username;
                    j["password"] = password;
                    std::string payload = j.dump();

                    std::string url = base_url + "/login";
                    std::promise<Infra::GatewayService::LoginResponse> promise;
                    http_.Post(url.c_str(), &payload, [&promise](const std::string &data) {
                        try {
                            auto j = nlohmann::json::parse(data);
                            std::string token = j.at("session").get<std::string>();
                            LoginResponse data = {
                                .host = j.at("host").get<std::string>(),
                                .port = j.at("port").get<int>(),
                                .token = j.at("session").get<std::string>()
                           
                            };
                            printf("GatewayService::Login: Received token: %s\n", token.c_str());
                            promise.set_value(data);
                        } catch (const std::exception& e) {
                            printf("GatewayService::Login: Error: %s\n", e.what());
                        }
                    });
                    return promise.get_future();
                }

            private:
                std::string base_url;
                T http_;
        };
    }
}