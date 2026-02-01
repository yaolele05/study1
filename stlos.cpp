//结合 OS 课程，从 “对象 vs 物理内存”
/*
操作系统只认识“内存页 / 字节”
C++ 才引入了“对象、构造、析构”这一层抽象
在 OS 眼里：
内存 = 一堆 物理页（page）
每个进程看到的是 虚拟地址空间
OS 负责：
1页表映射2回收页3进程结束时统一清理
OS视角：
地址 0x1000 ～ 0x2000：已分配
地址 0x2000 ～ 0x3000：空闲
*/
delete p;
//C++视角
p->~T();               // C++ 对象层
operator delete(p);    // 内存管理层 → 最终到 OS
//C++ 负责调用析构函数，释放对象占用的资源
//OS 负责回收内存页
//new/delete 只负责“分配/释放内存”，不负责“构造/析构对象”
//new T() = operator new + T::T()
//delete p = p->~T() + operator delete
//STL 容器的内存管理
//析构函数释放 p 指向的那块内存清理对象“内部资源”
//OS 还认为那块地址是“已分配”，因为 OS 只看得到那次 new / delete
//析构时为什么不能一步到位？因为：
//1.OS 只管“这块内存能不能回收”2.C++ 必须先保证：所有对象逻辑上“结束生命”资源不泄漏
/*
A* p = new A;
operator delete(p); // ❌
OS 角度：
内存释放了 
C++ 角度：
析构函数没执行 ❌
p 里的资源泄漏
文件 / 锁 / 内存全炸
*/
//内存还被占着 =内存泄漏（但不是对象泄漏）
//RAII 是 C++ 管理资源的核心原则：
/*
资源可以是：
内存，文件句柄，锁，套接字 / 线程 / GPU 资源
RAII 对象：
构造时获得资源
析构时释放资源
*/
/*
1. 内存管理与操作系统
堆与栈：C++程序的内存管理涉及操作系统的堆和栈。栈内存由操作系统自动管理，而堆内存通常由程序员通过new和delete来管理。操作系统通过提供系统调用（如malloc、free）来实现堆内存的分配与回收，C++的new和delete是对这些系统调用的封装。
虚拟内存：操作系统使用虚拟内存来为每个进程提供独立的地址空间。在C++中，程序员访问的是虚拟地址空间，操作系统会将这些地址映射到物理内存或硬盘上的交换文件中。
内存泄漏与操作系统：C++中的内存泄漏问题，即通过new分配的内存没有被delete释放，导致无法回收的内存。操作系统本身并不直接管控内存泄漏，但它提供的垃圾回收机制（如内存分页、虚拟内存管理）在某些程度上能减轻这种问题。

*/
#include <iostream>

int main() {
    // 在堆上分配内存
    int* ptr = new int(42);  // 动态分配
    
    std::cout << "Heap allocated value: " << *ptr << std::endl;

    // 释放内存
    delete ptr;
    
    return 0;
}
/*
2. 系统调用
在C++中，使用标准库或直接调用操作系统的系统调用接口来进行文件操作，系统调用进行文件读写。
*/
#include <iostream>
#include <fcntl.h>  // open()
#include <unistd.h> // read(), write(), close()

int main() {
    // 打开文件
    int fd = open("example.txt", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // 向文件写入数据
    const char* text = "Hello, OS!";
    if (write(fd, text, 12) == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }

    // 重新定位文件指针到文件开头
    lseek(fd, 0, SEEK_SET);

    // 从文件读取数据
    char buffer[13];
    if (read(fd, buffer, 12) == -1) {
        perror("Error reading from file");
        close(fd);
        return 1;
    }
    buffer[12] = '\0'; // 确保字符串结束

    std::cout << "Read from file: " << buffer << std::endl;

    // 关闭文件
    close(fd);

    return 0;
}
/*
3.多线程操作：<thread>库创建多线程，并与操作系统进行交互
*/
#include <iostream>
#include <thread>
#include <chrono>

void task1() {
    std::this_thread::sleep_for(std::chrono::seconds(2));  // 模拟耗时任务
    std::cout << "Task 1 completed." << std::endl;
}

void task2() {
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 模拟耗时任务
    std::cout << "Task 2 completed." << std::endl;
}

int main() {
    std::cout << "Starting tasks..." << std::endl;

    // 创建两个线程并启动
    std::thread t1(task1);
    std::thread t2(task2);

    // 等待两个线程执行完毕
    t1.join();
    t2.join();

    std::cout << "All tasks completed." << std::endl;

    return 0;
}
//5. 文件锁操作：使用文件锁来同步文件访问
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>  // flock()

int main() {
    int fd = open("lockfile.txt", O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // 对文件加锁，使用非阻塞锁
    if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
        perror("Error locking file");
        close(fd);
        return 1;
    }

    std::cout << "File locked successfully, performing operations..." << std::endl;

    // 模拟文件操作
    sleep(5);  // 等待5秒钟，模拟操作

    // 解锁文件
    if (flock(fd, LOCK_UN) == -1) {
        perror("Error unlocking file");
        close(fd);
        return 1;
    }

    std::cout << "File unlocked." << std::endl;

    close(fd);
    return 0;
}
