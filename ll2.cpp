#include <iostream>
using namespace std;
//用c++写文件读写
#include <unistd.h>//read write close
#include <fcntl.h>//open
#include <cstdio>//perr
int main(int argc,char*argv[])
{
     if(argc!=3)//必须接收两个参数（argv[0]命令名是源文件和目标文件）
     {
       cerr<<"usage:"<<argv[0]<<" src dst"<<endl;
       return 1;
     }
     //打开源文件
     int fd_src=open( argv[1], O_RDONLY);
     if(fd_src==-1)
     {
        perror("open src");
        return 1;
     }
     //打开/创建目标文件
     //O_WRONLY ：只写打开
     //O_CREAT ：文件不存在就创建
     //O_TRUNC：如果文件已经存在，把文件长度截断为 0
     int fd_dest=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC,0644);//0644：八进制权限位0是八进制，6是管理者权限，4是组权限，4是其他用户权限
     if(fd_dest==-1)
     {
        perror("open dest");
        close(fd_src);
        return 1;
     }
     char buffer[1024];
     ssize_t n;
     //循环读取源文件，写入目标文件
     while((n=read(fd_src,buffer,sizeof(buffer)))>0)
     {
        ssize_t ret=write(fd_dest,buffer,n);
        if(ret !=n)//检查写入是否成功（如果未写完）
        {
            perror("write");
            break;
        }

     }
     if(n<0)
     {
        perror("read");
     }
        //关闭文件描述符
        close(fd_src);
        close(fd_dest);
        return 0;
}