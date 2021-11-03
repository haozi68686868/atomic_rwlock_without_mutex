#include <cstdlib>
#include <cassert>
#include <atomic>
#include <thread>
#include "raii.hpp"
/*
 * atomic实现读写资源锁,独占写,共享读,禁止复制构造函数和'='赋值操作符
 * WRITE_FIRST为true时为写优先模式,如果有线程等待读取(m_writeWaitCount>0)则等待,优先让写线程先获取锁
 * 允许嵌套加锁
 * readLock/Unlock 实现共享的读取加/解锁，线程数不限
 * writeLock/Unlock 实现独占的写入加/解锁,同时只允许一个线程写入，
 * 当有线程在读取时，写入线程阻塞，当写入线程执行时，所有的读取线程都被阻塞。
 */
class RWLock {
#define WRITE_LOCK_STATUS -1
#define FREE_STATUS 0
private:
    /* 初始为0的线程id */
    static const  std::thread::id NULL_THEAD;
    const bool WRITE_FIRST;
    /* 用于判断当前是否是写线程 */
    std::thread::id m_write_thread_id;
    /* 资源锁计数器,类型为int的原子成员变量,-1为写状态，0为自由状态,>0为共享读取状态 */
    std::atomic_int m_lockCount;
    /* 等待写线程计数器,类型为unsigned int的原子成员变量*/
    std::atomic_uint m_writeWaitCount;
public:
    // 禁止复制构造函数
    RWLock(const RWLock&) = delete;
    // 禁止对象赋值操作符
    RWLock& operator=(const RWLock&) = delete;
    //RWLock& operator=(const RWLock&) volatile = delete;
    RWLock(bool writeFirst=false);;//默认为读优先模式
    virtual ~RWLock()=default;
    int readLock();
    int readUnlock();
    int writeLock();
    int writeUnlock();
    // 将读取锁的申请和释放动作封装为raii对象，自动完成加锁和解锁管理
    raii read_guard()const noexcept{
        return make_raii(*this,&RWLock::readUnlock,&RWLock::readLock);
    }
    // 将写入锁的申请和释放动作封装为raii对象，自动完成加锁和解锁管理
    raii write_guard()noexcept{
        return make_raii(*this,&RWLock::writeUnlock,&RWLock::writeLock);
    }
};
