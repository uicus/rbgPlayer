#ifndef CONCURRENT_QUEUE
#define CONCURRENT_QUEUE

// based on: https://github.com/Pixinn/misc/concurrent_deque

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class concurrent_queue{
        std::queue<T> collection;
        std::mutex mutex;
        std::condition_variable new_data_notifier;
    public:
        template<typename... Args>
        void emplace_back(Args&&... args){
            std::unique_lock<std::mutex> lock{mutex};
            collection.emplace(std::forward<Args>(args)...);
            lock.unlock();
            new_data_notifier.notify_one();
        }

        void clear(void){
            std::lock_guard<std::mutex> lock(mutex);
            collection.clear();
        }

        size_t size(void){
            std::unique_lock<std::mutex> lock{mutex};
            return collection.size();
        }

        T pop_front(void)noexcept{
            std::unique_lock<std::mutex> lock{mutex};
            while(collection.empty())
                new_data_notifier.wait(lock);
            auto elem = std::move(collection.front());
            collection.pop();
            return elem;
        }
};

#endif
