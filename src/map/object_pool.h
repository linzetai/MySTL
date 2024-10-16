#pragma once
#include <vector>
template <typename T, int Capacity>
class ObjectPool {
public:
    ObjectPool() {
        pool.reserve(Capacity);
        for (int i = 0; i < Capacity; ++i) {
            pool.push_back(new T());
        }
    }
    T* allocate() {
        if (!pool.empty()) {
            T* obj = pool.back();
            pool.pop_back();
            return obj;
        }
        return new T();
    }

    void deallocate(T* obj) {
        *obj = T{};
        pool.push_back(obj);
    }

    ~ObjectPool() {
        for (T* obj : pool) {
            delete obj;
        }
    }

private:
    std::vector<T*> pool;
};
