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
             signal(SIGINT,SIG_DFL);

            // 输入重定向
            if(i==0 && !pl.inf.empty()){
                int fd=open(pl.inf.c_str(),O_RDONLY);
                if(fd==-1){ perror("open inf"); exit(1); }
                dup2(fd,STDIN_FILENO);
                close(fd);
            }

            // 输出重定向
            if(i==n-1 && !pl.outf.empty()){
                int fd;
                if(pl.append)
                    fd=open(pl.outf.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
                else
                    fd=open(pl.outf.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd==-1){ perror("open outf"); exit(1); }
                dup2(fd,STDOUT_FILENO);
                close(fd);
            }

            // 管道重定向
            if(i>0)
                dup2(pipes[i-1][0],STDIN_FILENO);

            if(i<n-1)
                dup2(pipes[i][1],STDOUT_FILENO);

            // 关闭所有 pipe
            for(int j=0;j<n-1;j++){
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // exec
            vector<char*> argv;
            for(auto &s : pl.cmds[i].argv)
                argv.push_back(const_cast<char*>(s.c_str()));
            argv.push_back(nullptr);

            execvp(argv[0],argv.data());
            perror("execvp");
            exit(1);
        }

       
          pids.push_back(pid);
    }
        // 父进程关闭 pipe
    for(int i=0;i<n-1;i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // 父进程等待
    if(pl.backg==false){
        for(auto pid : pids)
            waitpid(pid,nullptr,0);
    }

}
//前台进程组 = 当前拥有终端控制权的进程组
int main()
{
    signal(SIGCHLD,reap);
    signal(SIGINT,SIG_IGN);//shell忽略ctrl+c
   string line;
   while(true)
   {
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));//cwd是当前工作目录的路径，getcwd函数将当前工作目录的路径复制到cwd数组中，sizeof(cwd)是cwd数组的大小，防止缓冲区溢出
    cout<<cwd<<"$: ";
    if(!getline(cin,line)) break;
    if(line.empty()) continue;
    auto pl=pline(line);
    //built-in cd
    if(pl.cmds.size()==1&&pl.backg==false)//为什么要判断后台执行？因为如果是后台执行，父进程不等待子进程结束，子进程执行cd后退出，不会影响父进程的当前目录
    {
        auto&cmd=pl.cmds[0];
        if(cmd.argv[0].empty()==false)
        {
        if(cmd.argv[0]=="cd")
        {
            const char* path=cmd.argv.size()>1?cmd.argv[1].c_str():getenv("HOME");//如果没有参数，默认切换到HOME目录//getenv详细讲解？
            if(chdir(path)==-1)
            {
                perror("cd");
            }
            continue;
        }
        if(cmd.argv[0]=="exit")
        {
            exit(0);
        }
        if(cmd.argv[0]=="pwd")//pwd命令显示当前工作目录
        {
            cout<<cwd<<endl;
            continue;
        }
     }
   }
    run(pl);
   }


    return 0;
}