#include <iostream>
#include <fcntl.h>      // open
#include <unistd.h>     // read, write, close
#include <cstdio>       // perror

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "usage: " << argv[0] << " src dst" << endl;
        return 1;
    }

    // 1. 打开源文件
    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1)
    {
        perror("open src");
        return 1;
    }

    // 2. 打开 / 创建目标文件
    int fd_dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dst == -1)
    {
        perror("open dst");
        close(fd_src);
        return 1;
    }

    char buffer[1024];
    ssize_t n;

    // 3. 循环读写
    while ((n = read(fd_src, buffer, sizeof(buffer))) > 0)
    {
        ssize_t ret = write(fd_dst, buffer, n);
        if (ret != n)
        {
            perror("write");
            break;
        }
    }

    if (n < 0)
    {
        perror("read");
    }

    // 4. 关闭
    close(fd_src);
    close(fd_dst);

    return 0;
}
