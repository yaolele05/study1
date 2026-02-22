#include <unistd.h>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
using namespace std;
struct command
{
    vector<string> argv;
};//单个命令
struct allline
{
    vector<command>cmds;
    string inf;
    string outf; 
    bool append=false;
    bool backg=false;
};
void reap(int)
{
    //回收
    while(waitpid(-1,nullptr,WNOHANG)>0);
}
//分词
vector<string>fen(const string &line )
{

   stringstream ss(line);
   string token;
   vector<string>tokens;
   while(ss>>token)
   {
     tokens.push_back(token);
   } 
   return  tokens;
}
//解析
allline pline(const string &line)
{
    allline pl;
    auto tokens=fen(line);
    command cmd;
    for(size_t i=0;i<tokens.size();i++)
    {
        if(tokens[i]=="<")
        {
            if(i+1<tokens.size())
            pl.inf=tokens[++i];

        }
        else if(tokens[i]==">")
        {
            if(i+1<tokens.size())
            {
                pl.outf=tokens[++i];
               
            }
        }
        else if(tokens[i]==">>")
        {
            pl.outf=tokens[++i];
            pl.append=true;
        }
        else if(tokens[i]=="|")
        {
            pl.cmds.push_back(cmd);
            cmd.argv.clear();
        }
        else if(tokens[i]=="&")
        {
            pl.backg=true;
        }
        else
        cmd.argv.push_back(tokens[i]);
    }
    if(cmd.argv.empty()==false)
    pl.cmds.push_back(cmd);

    return pl;
}
//执行
// 创建子进程 建立管道 重定向输入输出 执行命令 支持后台执行
void run(const allline&pl)
{
    int n=pl.cmds.size();
    vector<vector<int>>pipes(n-1,vector<int>(2));
     vector<pid_t>pids;
    for(int i=0;i<n;i++)
    {
        if(i<n-1)
        {
            if(pipe(pipes[i].data())==-1)
            {
                perror("pipe");
                exit(1);
            }
        }
        pid_t pid=fork();
       
        if(pid==-1)
        {
            perror("fork");
            exit(1);
        }
         else if(pid==0)
        {
             //子进程
             //恢复ctrl+c
             signal(SIGINT,SIG_DFL);
            //输入重定向
            if(i==0&&pl.inf.empty()==false)
            {
                int fd=open(pl.inf.c_str(),O_RDONLY);
                if(fd=-1)
                {
                    perror("open inf");
                    exit(1);
                }
                dup2(fd,STDIN_FILENO);
                close(fd);
            }
            if(i==n-1&&pl.outf.empty()==false)
            {
                int fd;
                if(pl.append)
                fd=open(pl.outf.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
                else
                fd=open(pl.outf.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd==-1)
                {
                    perror("open outf");
                    exit(1);
                }
                dup2(fd,STDOUT_FILENO);
                close(fd);
            }
            //管道重定向
          if(i>0)
          {
            dup2(pipes[i-1][0],STDIN_FILENO);
          }
          if(i<n-1)
          {
            dup2(pipes[i][1],STDOUT_FILENO);
          }
             
        }

          pids.push_back(pid);
    }
    for(int i=0;i<n-1;i++)
   {
    close(pipes[i][0]);
    close(pipes[i][1]);

   }
   if(pl.backg==false)
   {
    for(auto pid:pids)
    {
        waitpid(pid,nullptr,0);
    }
   }
}
