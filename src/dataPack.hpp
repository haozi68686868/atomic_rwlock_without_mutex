#include <memory>
#include <mutex>
#include <atomic>
#include "rwlock.hpp"

template <typename T>
class DataPack
{
public:
    using DataPtr = std::shared_ptr<T>;
    using DataConstPtr = std::shared_ptr<const T>;
    inline DataPtr getDataPtr()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return data_ptr;
    }
    inline void updatePack(const DataPtr &ptr)
    {
        std::lock_guard<std::mutex> lock(mtx);
        data_ptr = ptr;
    }

private:
    std::mutex mtx;
    DataPtr data_ptr;
};

template <typename T>
class DataPack2
{
public:
    using DataVarPtr = std::shared_ptr<T>;
    using DataConstPtr = std::shared_ptr<const T>;
    using DataPtr = DataConstPtr;
    inline DataPtr getDataPtr()
    {
        auto raii = rwlock.read_guard();
        return data_ptr;
    }
    inline void updatePack(const DataPtr &ptr)
    {
        // std::lock_guard<std::mutex> lock(mtx);
        auto raii = rwlock.write_guard();
        data_ptr = ptr;
    }

private:
    // std::mutex mtx;
    RWLock rwlock{false};// false = readFirst
    DataPtr data_ptr;
};