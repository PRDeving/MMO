#pragma once

#include <atomic>
#include <vector>

template<typename T>
class SPMCQueue {
    public:
        SPMCQueue(size_t max) : s(max), buffer(max), r(0), w(0) {}
        bool Push(T& data) {
            auto write = w.load(std::memory_order_relaxed);
            auto read = r.load(std::memory_order_acquire);
            if (write - read >= s) return false;

            buffer[write % s] = std::move(data);
            w.store(write + 1, std::memory_order_release);
            return true;
        }

        T* Pop() {
            while (true) {
                auto read = r.load(std::memory_order_relaxed);
                auto write = w.load(std::memory_order_acquire);
                if (read >= write) return nullptr;
                
                if (r.compare_exchange_weak(read, read + 1, std::memory_order_acquire, std::memory_order_relaxed)) {
                    return &buffer[read % s];
                }
            }
        }

        size_t size() {
            return w.load(std::memory_order_relaxed) - r.load(std::memory_order_relaxed);
        }
    private:
        const size_t s;
        std::vector<T> buffer;
        std::atomic<uint64_t> r;
        std::atomic<uint64_t> w;
};