#include <string>
#include <atomic>
#include <mutex>
#include <curl/curl.h>
#include "networking/impl/cUrl.hpp"

std::atomic_bool Networking::Impl::CurlManager::initialized(false);
std::mutex Networking::Impl::CurlManager::init_mutex;

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

Networking::Impl::cUrl::cUrl() {
    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("cUrl: Failed to create cUrl instance.");
    }
    printf("cUrl: Created cUrl instance.\n");
}

Networking::Impl::cUrl::~cUrl() {
    if (curl) curl_easy_cleanup(curl);
    printf("cUrl: Cleaned up cUrl instance.\n");
}

void Networking::Impl::cUrl::Get(const std::string &url, Networking::HTTP::Callback &callback) {
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        callback("CurlHTTP GET request failed: " + std::string(curl_easy_strerror(res)));
    } else {
        callback(response);
    }
    curl_easy_reset(curl);
}

void Networking::Impl::cUrl::Post(const std::string &url, const std::string *data, Networking::HTTP::Callback &callback) {
    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    if (data) curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data->c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        callback("CurlHTTP POST request failed: " + std::string(curl_easy_strerror(res)));
    } else {
        callback(response);
    }
    curl_slist_free_all(headers);
}