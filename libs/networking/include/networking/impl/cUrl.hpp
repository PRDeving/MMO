#pragma once
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include <atomic>
#include <mutex>
#include "../http.hpp"

namespace Networking {
    namespace Impl {
        class CurlManager {
        public:
            static CurlManager* getInstance() {
                static CurlManager instance;
                return &instance;
            }
            CurlManager(const CurlManager&) = delete;
            CurlManager& operator=(const CurlManager&) = delete;

        private:
            CurlManager() {
                std::lock_guard<std::mutex> lock(init_mutex);
                if (!initialized.load()) {
                    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
                    if (res != CURLE_OK) {
                        throw std::runtime_error("cUrl: Failed to initialize libcurl.");
                    }
                    initialized.store(true);
                    printf("cUrl: Initialized libcurl.\n");
                }
            }

            ~CurlManager() {
                std::lock_guard<std::mutex> lock(init_mutex);
                if (initialized.load()) {
                    curl_global_cleanup();
                    initialized.store(false);
                    printf("cUrl: Cleaned up libcurl.\n");
                }
            }

            static std::atomic_bool initialized;
            static std::mutex init_mutex;
        };


        class cUrl {
        public:
            cUrl();
            ~cUrl();
            void Get(const std::string &url, Networking::HTTP::Callback &callback);
            void Post(const std::string &url, const std::string *data, Networking::HTTP::Callback &callback);
        private:
            CurlManager *curlManager = CurlManager::getInstance();
            CURL *curl;
        };
    }
}