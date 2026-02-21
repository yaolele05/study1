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