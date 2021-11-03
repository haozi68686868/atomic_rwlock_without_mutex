# atomic_rwlock_without_mutex
A test program for different solutions for multi-Thread data

#### dataPack.hpp

有两种适用于多线程读写的数据模版封装

- 特点：ZeroCopy（零拷贝），数据通过智能指针shared_ptr来管理

- DataPack
  - 使用std::mutex来实现，高效便捷
- DataPack2
  - 使用Atomic原语操作来进行无锁编程（lock-free），内部实现了一个读写锁
  - CAS原语（Compare and Swap）
  - https://www.cnblogs.com/caca/p/lock-free_CAS_ABA.html
  - 内部rwlock（读写锁，支持读优先和写优先配置）的实现摘自热心网友
    - https://blog.csdn.net/mw_nice/article/details/85257136
    - 修改项：原实现做了一些防止无效加锁的防错机制(通过判断thread::id)，修改后删掉了这部分代码来提高代码效率，不过需要自己注意加锁和解锁。

#### 效率测试

- DataPack在通常情况效率都不错
- 在单一线程写入，多个线程读取的情况，DataPack2效率明显高，得益于其读写锁的特性（读锁可以共享），以及其非阻塞的特性
- 在有多个线程写入的情况，DataPack2效率明显变差

#### 编译和运行

```bash
mkdir build
cd build
cmake ..
make
../bin/multiThread_Test
```

