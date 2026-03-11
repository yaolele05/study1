#include <unistd.h>
#include <bits/stdc++.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <termios.h> // 新增：终端模式头文件
using namespace std;

// 单个命令结构
struct command
{
    vector<string> argv;
    string inf;            
    string outf;             
    bool append = false;  
};

// 整行命令解析结果
struct allline
{
    vector<command> cmds;    
    bool bg = false;
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
            if (!cur.empty())
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
    if (!cur.empty())
        tokens.push_back(cur);

    return tokens;
}

allline pline(const string &line)
{
    allline pl;
    auto tokens = fen(line);
    command cmd;

    // 处理后台运行标记
    if (!tokens.empty() && tokens.back() == "&")
    {
        pl.bg = true;
        tokens.pop_back();
    }

    for (size_t i = 0; i < tokens.size(); i++)
    {
        const string &tok = tokens[i];
        
        // 输入重定向 <
        if (tok == "<")
        {
            if (i+1 < tokens.size())
                cmd.inf = tokens[++i];
        }
        // 输出重定向 >
        else if (tok == ">")
        {
            if (i+1 < tokens.size()) {
                cmd.outf = tokens[++i];
                cmd.append = false; // 重置追加标记
            }
        }
        // 输出追加 >>
        else if (tok == ">>")
        {
            if (i+1 < tokens.size())
            {
                cmd.outf = tokens[++i];
                cmd.append = true;
            }
        }
        // 管道 | - 关键修复：重置整个command，而非仅清空argv
        else if (tok == "|")
        {
            pl.cmds.push_back(cmd);
            cmd = command(); // 重置所有属性（包括重定向）
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
            // ~ 路径展开 - 修复指针拼接错误
            if (arg.size() >= 1 && arg[0] == '~')
            {
                const char *home = getenv("HOME");
                if (home)
                {
                    if (arg.size() == 1) {
                        arg = home;
                    } else {
                        arg = string(home) + arg.substr(1); // C++字符串拼接
                    }
                }
            }

            cmd.argv.push_back(arg);
        }
    }
    if (!cmd.argv.empty())
        pl.cmds.push_back(cmd);

    return pl;
}

void run(allline& pl)
{
    int n = pl.cmds.size();
    if (n == 0) return;

    vector<vector<int>> pipes(n-1, vector<int>(2));
   
    // 创建管道
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
    bool is_bg = pl.bg;  

    // 保存父进程终端属性（用于嵌套Shell恢复）
    struct termios orig_term;
    tcgetattr(STDIN_FILENO, &orig_term);

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
            // 设置进程组
            if (i == 0)
                setpgid(0, 0);
            else
                setpgid(0, pgid);

            // 恢复终端默认模式（解决嵌套Shell无回显）
            tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
            // 子进程接管终端控制权（关键：支持嵌套Shell）
            tcsetpgrp(STDIN_FILENO, getpgid(0));

            // 信号调整：适配嵌套Shell
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGCHLD, SIG_DFL);
            signal(SIGTTIN, SIG_IGN); // 忽略终端输入信号，允许子Shell接管
            signal(SIGTTOU, SIG_IGN); // 忽略终端输出信号，允许子Shell接管

            // 输入重定向 - 关键修复：移除i==0限制，支持任意命令的输入重定向
            if (!pl.cmds[i].inf.empty())
            {
                int fd = open(pl.cmds[i].inf.c_str(), O_RDONLY);
                if (fd == -1) { perror("open inf"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // 输出重定向 - 关键修复：移除i==n-1限制，支持任意命令的输出重定向
            if (!pl.cmds[i].outf.empty())
            {
                int flags = O_WRONLY | O_CREAT;
                flags |= pl.cmds[i].append ? O_APPEND : O_TRUNC;
                int fd = open(pl.cmds[i].outf.c_str(), flags, 0644);
                if (fd == -1) { perror("open outf"); exit(1); }
                // 同时重定向stdout和stderr（符合系统Shell行为）
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);
            }

            // 管道输入（非第一个命令）
            if (i > 0)
            {
                dup2(pipes[i-1][0], STDIN_FILENO);
                close(pipes[i-1][0]);
                close(pipes[i-1][1]);
            }

            // 管道输出（非最后一个命令）
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
                pgid = pid;
            setpgid(pid, pgid);
            pids.push_back(pid);

            // 关闭管道描述符
            if (i < n-1)
                close(pipes[i][1]);
            if (i > 0)
                close(pipes[i-1][0]);
        }
    }

    // 父进程关闭剩余管道描述符
    for (int j = 0; j < n-1; j++)
    {
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    // 等待子进程（支持嵌套Shell）
    if(!is_bg)
    {
        pid_t original_pgid = tcgetpgrp(STDIN_FILENO);
        tcsetpgrp(STDIN_FILENO, pgid);
        
        int status;
        for (pid_t pid : pids)
        {
            // 关键：支持子进程暂停/恢复（嵌套Shell需要）
            waitpid(pid, &status, WUNTRACED | WCONTINUED);
        }
        
        tcsetpgrp(STDIN_FILENO, original_pgid);
    }
}

int main()
{ 
    const char* home_env = getenv("HOME");
    string home = home_env ? home_env : "/";
    chdir(home.c_str()); 
    setpgid(0, 0);
    tcsetpgrp(STDIN_FILENO, getpgrp());

    // 信号处理
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGCHLD, (sighandler_t)reap);

    string line;
    string oldpwd = getenv("HOME");

    while (true)
    {
        reap();  
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        string path = cwd;
        
        // 路径美化（~替换HOME）
        if (path.find(home) == 0)
        {
            path = "~" + path.substr(home.size());  
        }
        cout << "\033[1;32m" << path << "\033[34m $: \033[0m" << flush;

        // 读取输入（容错处理）
        if (!getline(cin, line)) {
            if (cin.eof()) break; 
            continue;
        }
        if (line.empty()) continue;

        // 解析命令
        allline pl = pline(line);
        bool is_builtin = false;

        // 处理内建命令
        if (pl.cmds.size() == 1)
        {
            auto &cmd = pl.cmds[0].argv;
            if (!cmd.empty())
            {
                is_builtin = true;
                // 备份文件描述符
                int stdout_bak = dup(STDOUT_FILENO);
                int stderr_bak = dup(STDERR_FILENO);

                // 内建命令重定向（支持追加）
                if (!pl.cmds[0].outf.empty())
                {
                    int flags = O_WRONLY | O_CREAT;
                    flags |= pl.cmds[0].append ? O_APPEND : O_TRUNC;
                    int fd = open(pl.cmds[0].outf.c_str(), flags, 0644);
                    if (fd != -1)
                    {
                        dup2(fd, STDOUT_FILENO);
                        dup2(fd, STDERR_FILENO); // 内建命令也重定向stderr
                        close(fd);
                    }
                }

                // 处理cd
                if (cmd[0] == "cd")
                {
                    string tar;
                    if (cmd.size() == 1)
                        tar = home;
                    else if (cmd[1] == "-")
                    {
                        if (oldpwd.empty())
                            cerr << "cd: OLDPWD not set" << endl;
                        else
                        {
                            tar = oldpwd;
                            string spath = tar;
                            if (spath.find(home) == 0)
                                spath = "~" + spath.substr(home.size());
                            cout << spath << endl;
                        }
                    }
                    else
                        tar = cmd[1];

                    char cur_dir[1024];
                    getcwd(cur_dir, sizeof(cur_dir));
                    if (chdir(tar.c_str()) == -1)
                        perror("cd");
                    else
                        oldpwd = cur_dir;
                }
                // 处理exit
                else if (cmd[0] == "exit")
                {
                    exit(0);
                }
                // 处理pwd
                else if (cmd[0] == "pwd")
                {
                    char pwd_cwd[1024];
                    getcwd(pwd_cwd, sizeof(pwd_cwd));
                    cout << pwd_cwd << endl;
                }
                // 处理echo
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
                // 非内建命令
                else
                {
                    is_builtin = false;
                }

                // 恢复文件描述符（容错）
                if (stdout_bak != -1) {
                    dup2(stdout_bak, STDOUT_FILENO);
                    close(stdout_bak);
                }
                if (stderr_bak != -1) {
                    dup2(stderr_bak, STDERR_FILENO);
                    close(stderr_bak);
                }

                if (is_builtin) continue;
            }
        }

        // 执行非内建命令
        if (!is_builtin)
        {
            run(pl);
        }
    }

    cout << endl;
    return 0;
}