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
void reapc(int)
{
    //回收已经退出的子进程
    while(waitpid(-1,nullptr,WNOHANG)>0);
}
vector<string>fen(const string &line)
{
    stringstream ss(line);
    string token;
    vector<string>tokens;
    while(ss>>token)
    {
        tokens.push_back(token);
    }
    return tokens;
}
//解析命令行
allline pline(const string&line)
{
    allline pl;//
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
                pl.append=true;
            }
        }
        else if(tokens[i]==">>")
        {
            if(i+1<tokens.size())
            {
                pl.outf=tokens[++i];
                pl.append=true;

            }
        }
        else if(tokens[i]=="|")
        {
            pl.cmds.push_back(cmd);
            cmd.argv.clear();//
        }
        else if(tokens[i]=="&")
        {
            pl.backg=true;
        }
        else
        {
            cmd.argv.push_back(tokens[i]);
        }

    }
    if(cmd.argv.empty()==false)
     pl.cmds.push_back(cmd);

     return pl;
}
//执行
void execline(allline&pl)
{
    int n=pl.cmds.size();
    if(n==0)
    return;
    vector<int[2]>pipes(n-1);
    //创建管道(n-1)个
    for(int i=0;i<n;i++)
    {
        if(pipe(pipes[i])<0)
        {
            perror("pipe");
            return;
        }
    }
    vector<pid_t>pids;
    for(int i=0;i<n;i++)
    {
        pid_t pid=fork();
        if(pid<0)
        {
        perror("fork");
        return ;
        }
        if(pid==0)//在子进程
        {
          //子进程恢复ctrl+c；
          signal(SIGINT,SIG_DFL);
          //输入重定向
          if(i==0&&!pl.inf.empty())
          {
            int fd =open(pl.inf.c_str(),O_RDONLY);//open 参数
            if(fd<0)
            perror("open input");
            exit(1);//和return区别?
          }
          dup2(fd,STDIN_FILENO);
          close(fd);
        
        else if(i>0)
        {
            dup2(pipes[i-1][0],STDIN_FILENO);//pipes?
        }
        //输出重定向

        if(i==n-1&&pl.outf.empty()==false)
        {
         int f=O_WRONLY|O_CREAT|(pl.append?O_APPEND:O_TRUNC);
         int fd=open(pl.outf.c_str(),f,0644);
         if(fd<0)
         {
            perror("open");//he open output 区别
            exit(1);
         }
         dup2(fd,STDOUT_FILENO)//fd>0?
         close(fd);
        }
        else if(i<n-1)//n-1?
        {
          dup2(pipes[i][1],STDOUT_FILENO);
        }
        for(int j=0;j<n-1;j++)
        {
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
        vector<char*>argvc;//为什么不用vector<string>argvc?
        for(auto&s:pl.cmds[i].argv)
         argvc.push_back(&s[0]);
         argvc.push_back(nullptr);
         execvp(argvc[0],argvc.data());
         perror("execvp");
         exit(1);
       }
          pids.push_back(pid);
    }
   //关pipe
   for(int i=0;i<n-1;i++)
   {
    close(pipes[i][0]);
    close(pipes[i][0]);
   }
   //前台命令
   if(pl.backg==false)
   {
    for(auto pid:pids)
      waitpid(pid,nullptr,0);
   }
}


int main()
{
    signal(SIGINT,SIG_IGN);
    signal(SIGCHLD,reapc);//解释参数都可以怎么用，做什么？
    string lastd;
    while(true)
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        cout<<cwd<<"$";
        cout.flush();//什么意思
        string line;
        if(!getline(cin,line)) break;
        if(line.empty()) continue;
        if(line=="exit") break;
        allline pl=pline(line);
         //cd
        if(pl.cmds.empty()==false&&pl.cmds[0].argv[0]=="cd")
        {
            char now[1024];
            getcwd(now,sizeof(now));

            string path;
            if(pl.cmds[0].argv.size()==1)
            {
                path=getenv("HOME");//getenv?
            }
            else if(pl.cmds[0].argv[1]=="-")
            {
              if(lastd.empty())
              {
                cerr << "cd: OLDPWD not set\n";//?什么意思
                    continue;
              }
              path=lastd;
            }
            else 
            {
                path=pl.cmds[0].argv[1];
            }
            if(chdir(path.c_str())==0)//c_str到底什么意思，和open里面的pl.inf.c_str分别是什么？怎么用
            {
                lastd=now;
            }
            else
            {
                perror("cd");
            }
            continue;
        }
        exec(pl);
    }
    
    
    return 0;
}