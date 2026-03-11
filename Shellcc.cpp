#include <unistd.h>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
using namespace std;
// 单个命令结构
struct command
{
    vector<string> argv;
};
// 整行命令解析结果
struct allline
{
    vector<command> cmds;    
    string inf;            
    string outf;             
    bool append = false;    
};
// 回收子进程
void reap()
{
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0);
}
// 分词
vector<string> fen(const string &line)
{
    vector<string> tokens;
    string cur;
    for (char c : line)
    {
        // 空格分隔
        if (isspace(c))
        {
            if (cur.empty()==false)
            {
                tokens.push_back(cur);
                cur.clear();
            }
            continue;
        }
    
        // 普通字符
        cur.push_back(c);
    }

    // 处理最后一个token
    if (cur.empty()==false)
        tokens.push_back(cur);

    return tokens;
}
allline pline(const string &line)
{
    allline pl;
    auto tokens = fen(line);
    command cmd;

    for (size_t i = 0; i < tokens.size(); i++)
    {
        const string &tok = tokens[i];
        
        // 输入重定向 <
        if (tok == "<")
        {
            if (i+1 < tokens.size())
                pl.inf = tokens[++i];
        }
        // 输出重定向 >
        else if (tok == ">")
        {
            if (i+1 < tokens.size())
                pl.outf = tokens[++i];
        }
        // 输出追加 >>
        else if (tok == ">>")
        {
            if (i+1 < tokens.size())
            {
                pl.outf = tokens[++i];
                pl.append = true;
            }
        }
        // 管道 
        else if (tok == "|")
        {
            pl.cmds.push_back(cmd);
            cmd.argv.clear();
        }
        // 普通参数（处理环境变量/$HOME展开）
        else
        {
            string arg = tok;
            
            // 环境变量展开
            if (arg.size() > 1 && arg[0] == '$')
            {
                const char *val = getenv(arg.substr(1).c_str());
                arg = val ? val : "";
            }
            if (arg.size() > 1 && arg[0] == '~')
            {
                const char *home = getenv("HOME");
                if (home)
                    arg = home + arg.substr(1);
            }

            cmd.argv.push_back(arg);
        }
    }
    if (cmd.argv.empty()==false)
        pl.cmds.push_back(cmd);

    return pl;
}
void run( allline& pl)
{
    int n = pl.cmds.size();
    if (n == 0) return;

    vector<vector<int>> pipes(n-1, vector<int>(2));
   
    for (int i = 0; i < n-1; i++)
    {
        if (pipe(pipes[i].data()) == -1)
        {
            perror("pipe");
            exit(1);
        }
    }

    pid_t pgid = 0;
    vector<pid_t> pids;
     bool is_bg= false;
      if (!pl.cmds.empty() && pl.cmds.back().argv.back() == "&")
       {
        is_bg= true;
        pl.cmds.back().argv.pop_back();  // 去掉 '&'
    }
    for (int i = 0; i < n; i++)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        // 子进程
        if (pid == 0)
        {
            // 设置进程组（前台控制）
            if (i == 0)
                setpgid(0, 0);
            else
                setpgid(0, pgid);
            // 子进程响应Ctrl+C
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
        signal(SIGCHLD, SIG_DFL);  // 恢复子进程退出信号
        signal(SIGTTIN, SIG_DFL);  // 恢复终端输入信号
        signal(SIGTTOU, SIG_DFL);  // 恢复终端输出信号
            // 输入重定向
            if (i == 0 && !pl.inf.empty())
            {
                int fd = open(pl.inf.c_str(), O_RDONLY);
                if (fd == -1) { perror("open inf"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            // 输出重定向
            if (i == n-1 && !pl.outf.empty())
            {
                int fd = open(pl.outf.c_str(),
                    O_WRONLY | O_CREAT | (pl.append ? O_APPEND : O_TRUNC),
                    0644);
                if (fd == -1) { perror("open outf"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            // 管道输入，不是第一个命令
            if (i > 0)
            {
                dup2(pipes[i-1][0], STDIN_FILENO);
                close(pipes[i-1][0]);
                close(pipes[i-1][1]);
            }
            // 管道输出，非最后一个命令
            if (i < n-1)
            {
                dup2(pipes[i][1], STDOUT_FILENO);
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            // 关闭所有管道
            for (int j = 0; j < n-1; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // 执行命令
            vector<char*> argv;
            for (auto &s : pl.cmds[i].argv)
            {
                argv.push_back(const_cast<char*>(s.c_str()));
            }
            argv.push_back(nullptr);  

            execvp(argv[0], argv.data());  
            perror("execvp");  
            exit(1);
        }
        // 父进程逻辑
        else
        {
            if (i == 0)
                pgid = pid;  // 第一个子进程作为进程组组长
            setpgid(pid, pgid);
            pids.push_back(pid);
            if(n>1)
            {
            if (i < n-1)
            {
                close(pipes[i][1]);// 关闭当前管道写端
            }
            // 关闭上一个管道的读端
            if (i > 0)
            {
                close(pipes[i-1][0]);
            }
           }
        
        }
    }
    // 等待所有子进程结束
    if(is_bg==false)
    {
    int status;
    for (pid_t pid : pids)
    {
        waitpid(pid, &status, 0);
    }
   }
}
int main()
{ 
    const char* home_env = getenv("HOME");
    string home = home_env ? home_env : "/";
    chdir(home.c_str()); 
    setpgid(0, 0);
    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    string line;
    string oldpwd = getenv("HOME");  // 记录上一次的cd -

    while (true)
    {
        reap();  
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        string path = cwd;
        const char* home_env = getenv("HOME");
        string home = home_env ? home_env : "/";
        if (path.find(home) == 0)
        {
            path = "~" + path.substr(home.size());  
        }
        cout << "\033[1;32m" << path << "\033[34m $: \033[0m" << flush;

        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        // 解析命令
        allline pl = pline(line);
        bool is_b = false;

        // 处理内建命令
        if (pl.cmds.size() == 1)
        {
            auto &cmd = pl.cmds[0].argv;
            if (cmd.empty() == false)
            {
                is_b = true;
                int stdout_bak = dup(STDOUT_FILENO);
                int stderr_bak = dup(STDERR_FILENO);

                // 内建命令支持重定向
                if (!pl.outf.empty())
                {
                    int fd = open(pl.outf.c_str(),
                        O_WRONLY | O_CREAT | (pl.append ? O_APPEND : O_TRUNC),
                        0644);
                    if (fd != -1)
                    {
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    }
                }
                if (cmd[0] == "cd")
                {
                    string tar;
                    if (cmd.size() == 1)
                    {
                        tar = home;  // 默认切换到HOME
                    }
                    else if (cmd[1] == "-")
                    {
                        if (oldpwd.empty())
                        {
                            cerr << "cd: OLDPWD not set" << endl;
                        }
                        else
                        {
                            tar = oldpwd;
                            // 输出切换后的目录
                            string spath = tar;
                            if (spath.find(home) == 0)
                                spath = "~" + spath.substr(home.size());
                            cout << spath << endl;
                        }
                    }
                    else
                    {
                        tar = cmd[1];  // 普通路径
                    }

                    // 切换目录
                    char cur_dir[1024];
                    getcwd(cur_dir, sizeof(cur_dir));
                    if (chdir(tar.c_str()) == -1)
                    {
                        perror("cd");
                    }
                    else
                    {
                        oldpwd = cur_dir;  // 更新oldpwd
                    }

                }
                else if (cmd[0] == "exit")
                {
                    exit(0);
                }
                // pwd
                else if (cmd[0] == "pwd")
                {
                     char pwd_cwd[1024];
                    getcwd(pwd_cwd, sizeof(pwd_cwd));
                    cout << pwd_cwd << endl;
                   
                }
                // echo
                else if (cmd[0] == "echo")
                {
                    for (size_t i = 1; i < cmd.size(); i++)
                    {
                        cout << cmd[i];
                        if (i != cmd.size() - 1)
                            cout << " ";
                    }
                    cout << endl;

                }
                else
                {
                    is_b = false;  // 不是内建命令    
                    
                }

                    dup2(stdout_bak, STDOUT_FILENO);
                    dup2(stderr_bak, STDERR_FILENO);
                    close(stdout_bak);
                    close(stderr_bak);
                if(is_b) continue;
            }
        }
        // 非内建
        if (is_b==false)
        {
            run(pl);
        }
    }
    cout << endl;
    return 0;
}