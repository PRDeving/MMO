#pragma once
#include <atomic>
#include <vector>


template<typename T>
class MPSCQueue {
    public:
        MPSCQueue(size_t max) : size(max), buffer(max), r(0), w(0) {}
        bool Push(std::shared_ptr<T> data) {
            while (true) {
                auto write = w.load(std::memory_order_relaxed);
                auto read = r.load(std::memory_order_acquire);

                if (write - read >= size) return false;

                if (w.compare_exchange_weak(write, write + 1, std::memory_order_acquire, std::memory_order_relaxed)) {
                    buffer[write % size] = std::move(data);
                    return true;
                    break;
                }
            }
        }

        T* Pop() {
            auto read = r.load(std::memory_order_relaxed);
            auto write = w.load(std::memory_order_acquire);
            if (read >= write) return nullptr;
            auto data = &buffer[read % size];
            r.store(read + 1, std::memory_order_release);
            return data;
        }
    private:
        const size_t size;
        std::vector<T> buffer;
        std::atomic<uint64_t> r;
        std::atomic<uint64_t> w;
};