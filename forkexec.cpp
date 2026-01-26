#include <iostream>
#include <unistd.h>     // fork, execlp
#include <sys/wait.h>   // wait
#include <cstdio>       // perror
#include <fcntl.h>      // open
using namespace std;

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // 子进程
        cout << "child process, pid = " << getpid() << endl;

        execlp("ls", "ls", "-l", nullptr);

        // 只有 exec 失败才会执行到这里
        perror("exec");
        return 1;
    }
    else if (pid > 0)
    {
        // 父进程
        wait(nullptr);
        cout << "parent: child finished" << endl;
    }
    else
    {
        perror("fork");
        return 1;
    }

    return 0;
}
/*
fork：
  复制当前进程，生成一个几乎一模一样的子进程
fork的返回值：
  -1：表示 fork 失败
  >0：表示父进程，返回值是子进程的 pid
  =0：表示子进程，返回值是 0

exec（写在子进程里）：
  用新程序覆盖当前进程：用一个新程序，完全替换当前进程的代码和数据（1.不会创建新进程2.PID 不变3.只是“换程序”）
 
*/
//errno 与 perror
// 当系统调用失败时，会设置一个全局变量 errno，表示错误类型
// perror 可以根据 errno 打印错误信息

int main()
{
    int fd = open("not_exist.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    close(fd);
    return 0;
}
