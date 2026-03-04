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
    string errf;
    bool err_append=false;
    bool append=false;
    bool backg=false;
};
struct job
{
    int id;
    pid_t pgid;
    string cmdline;
    bool stopped;
};

vector<job> jobs;
int njid=1;//作业id从1开始
void reap(int)
{
    int status;
    pid_t pid;
    //回收
    while(pid=waitpid(-1,&status,WNOHANG)>0);
}
//分词
vector<string>fen(const string &line )
{

   vector<string> tokens;
   string cur;
   bool in_sig=false;
   bool in_dou=false;
   for(size_t i=0;i<line.size();i++)
   {
    char c=line[i];
    if(c==' '&&in_sig==false&&in_dou==false)
    {
        if(cur.empty()==false)
        {
            tokens.push_back(cur);
            cur.clear();
        }
    }
    if(c=='\'' &&in_dou==false)
        {
            in_sig=!in_sig;
            continue;
        }
    if(c=='"'&&in_sig==false)
    {   
        in_dou=!in_dou;
        continue;

    }
    if(isspace(c)&&in_sig==false&&in_dou==false)
    {
        if(cur.empty()==false)
        {
            tokens.push_back(cur);
            cur.clear();
        }
    }
    else
    {
        cur.push_back(c);
    }
  }
  if(cur.empty()==false)
  {
    tokens.push_back(cur);
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
        else if(tokens[i]=="2>")
        {
            if(i+1<tokens.size())
            {
                pl.errf=tokens[++i];
                
            }
        }
        else if(tokens[i]=="2>>")
        {
            if(i+1<tokens.size())
            {
                pl.errf=tokens[++i];
                pl.err_append=true;
            }
        }
        else
        {
            string arr=tokens[i];
            //环境变量展开(arr相当于路径)
            if(arr.size()>1&&arr[0]=='$')
            {
                const char*s=getenv(arr.substr(1).c_str());
                if(s)
                arr=s;
                else
                arr="";

            }
            if(arr.size()>1&&arr[0]=='~')
            {
                const char *s=getenv("HOME");
                if(s)
                arr=s+arr.substr(1);
            }
            cmd.argv.push_back(arr);
        }
    }
    if(cmd.argv.empty()==false)
    pl.cmds.push_back(cmd);

    return pl;
}
//执行
// 创建子进程 建立管道 重定向输入输出 执行命令 支持后台执行
void run(const allline&pl,const string &line)
{
    pid_t pgid=0;
    

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
            if(pgid==0)
            pgid=getpid();

            setpgid(0,pgid);//将子进程加入到pgid进程组中
             signal(SIGINT,SIG_DFL);
             signal(SIGTSTP,SIG_DFL);

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
            if(i==n-1 &&pl.errf.empty()==false)
            {
                int fd;
                if(pl.err_append==true)
                fd=open(pl.errf.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
                else
                fd=open(pl.errf.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
                if(fd==-1){perror("open errf");exit(1);}
                dup2(fd,STDERR_FILENO);
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
        else
        {
            //父进程
            if(pgid==0)
             pgid=pid;//第一次fork的子进程的pid作为pgid

             setpgid(pid,pgid);
        }

       
          pids.push_back(pid);
    }
        // 父进程关闭 pipe
    for(int i=0;i<n-1;i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
     if(pl.backg==false)
     tcsetpgrp(STDIN_FILENO,pgid);//将前台进程组设置为第一个子进程的进程组
     else
    // 父进程等待
    if(pl.backg==false)
    {
        int status;
        while(waitpid(-pgid,&status,WUNTRACED)>0)
         if(WIFSTOPPED(status))
         {
            jobs.push_back({njid++,pgid,line,1});//1表示停止
            cout<<"["<<njid-1<<"] "<<pgid<<" stopped"<<endl;
         }
         tcsetpgrp(STDIN_FILENO,getpgrp());//获取当前进程的进程组id
    }
    else
    {
        jobs.push_back({njid++,pgid,line,0});
        cout<<"["<<njid-1<<"] "<<pgid<<endl;
    }

     
}
//前台进程组 = 当前拥有终端控制权的进程组
int main()
{
   
    signal(SIGCHLD,reap);
    signal(SIGINT,SIG_IGN);//shell忽略ctrl+c
    signal(SIGTSTP,SIG_IGN);//shell忽略ctrl+z；
   string line;
   while(true)
   {
    char cwd[1024];
    getcwd(cwd,sizeof(cwd));//cwd是当前目录的路径，getcwd函数将当前工作目录的路径复制到cwd数组中
    cout<<cwd<<"$: ";
    if(!getline(cin,line)) break;
    if(line.empty()) continue;
    auto pl=pline(line);
    
    //内建  cd
    if(pl.cmds.size()==1&&pl.backg==false)
    {
        auto &cmd = pl.cmds[0];
        if(cmd.argv[0].empty()==false)
        {
             int stdoutnew= dup(STDOUT_FILENO);//内建命令可能搭配重定向，所以备份
             int stderrnew = dup(STDERR_FILENO);
             if(pl.cmds.empty()==false)
        {
        int fd;
        if(pl.append)
        fd=open(pl.outf.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
        else
        fd=open(pl.outf.c_str(),O_WRONLY|O_CREAT|O_TRUNC,0644);
        dup2(fd,STDOUT_FILENO);
        close (fd);

        }
        if(cmd.argv[0]=="cd")
        {
            const char* path=cmd.argv.size()>1?cmd.argv[1].c_str():getenv("HOME");//如果没有参数，默认切换到HOME目录//getenv详细讲解？
            if(chdir(path)==-1)
            {
                perror("cd");
            }
            continue;
        }
        if(cmd.argv[0]=="fg")
        {
            if(jobs.empty())
            {
                cout<<"none\n"<<endl;
                continue;
            }
            job j=jobs.back();
            jobs.pop_back();
            tcsetpgrp(STDIN_FILENO,j.pgid);//将前台进程组设置为j.pgid
            if(j.stopped)
            {
                kill(-j.pgid,SIGCONT);
            }
            int status;
            waitpid(-j.pgid,&status,WUNTRACED);//W..表示停止或者结束的
            tcsetpgrp(STDIN_FILENO,getpgrp());
             continue;
        }
        if(cmd.argv[0]=="echo")
        {
            for(size_t i=1;i<cmd.argv.size();i++)
            {
                cout<<cmd.argv[i];
                if(i!=cmd.argv.size()-1)
                cout<<" ";
            }
            cout<<endl;
            continue;//
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
        dup2(stdoutnew,STDOUT_FILENO);
        dup2(stderrnew,STDERR_FILENO);
        close(stdoutnew);
        close(stderrnew);
    }
    
     }
     run(pl,line);
   }
    return 0;
 }


   