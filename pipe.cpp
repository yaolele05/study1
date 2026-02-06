#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

struct Command {
    vector<string> argv;
};

struct Pipeline {
    vector<Command> cmds;
    string input_file;
    string output_file;
    bool append = false;
    bool background = false;
};

// 分词
vector<string> split(const string &line) {
    stringstream ss(line);
    string token;
    vector<string> tokens;
    while (ss >> token) tokens.push_back(token);
    return tokens;
}

// 解析命令
Pipeline parse_line(const string &line) {
    Pipeline pl;
    auto tokens = split(line);
    Command cmd;
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "<") {
            if (i + 1 < tokens.size()) pl.input_file = tokens[++i];
        } else if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) { pl.output_file = tokens[++i]; pl.append = false; }
        } else if (tokens[i] == ">>") {
            if (i + 1 < tokens.size()) { pl.output_file = tokens[++i]; pl.append = true; }
        } else if (tokens[i] == "|") {
            pl.cmds.push_back(cmd);
            cmd.argv.clear();
        } else if (tokens[i] == "&") {
            pl.background = true;
        } else {
            cmd.argv.push_back(tokens[i]);
        }
    }
    if (!cmd.argv.empty()) pl.cmds.push_back(cmd);
    return pl;
}

// 执行 pipeline
void execute_pipeline(Pipeline &pl) {
    int n = pl.cmds.size();
    vector<int[2]> pipes(n-1);

    for (int i = 0; i < n-1; i++) pipe(pipes[i]);

    vector<pid_t> pids;
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) { // 子进程
            // 输入
            if (i == 0 && !pl.input_file.empty()) {
                int fd = open(pl.input_file.c_str(), O_RDONLY);
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // 输出
            if (i == n-1 && !pl.output_file.empty()) {
                int flags = O_WRONLY | O_CREAT | (pl.append ? O_APPEND : O_TRUNC);
                int fd = open(pl.output_file.c_str(), flags, 0644);
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (i < n-1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // 关闭所有 pipe
            for (int j = 0; j < n-1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // 构造 exec 参数
            vector<char*> argv_c;
            for (auto &arg : pl.cmds[i].argv) argv_c.push_back(&arg[0]);
            argv_c.push_back(nullptr);

            execvp(argv_c[0], argv_c.data());
            perror("execvp");
            exit(1);
        }
        pids.push_back(pid);
    }

    // 父进程关闭 pipe
    for (int i = 0; i < n-1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    if (!pl.background) {
        for (auto pid : pids) waitpid(pid, nullptr, 0);
    }
}

int main() {
    signal(SIGINT, SIG_IGN); // Ctrl+C 不杀 shell

    while (true) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        cout << "myshell:" << cwd << " $ ";
        cout.flush();

        string line;
        if (!getline(cin, line)) break;
        if (line.empty()) continue;
        if (line == "exit") break;

        Pipeline pl = parse_line(line);

        // 内建 cd
        if (!pl.cmds.empty() && pl.cmds[0].argv[0] == "cd") {
            string path = pl.cmds[0].argv.size() > 1 ? pl.cmds[0].argv[1] : getenv("HOME");
            if (chdir(path.c_str()) != 0) perror("cd");
            continue;
        }

        execute_pipeline(pl);
    }
    return 0;
}
