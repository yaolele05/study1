#include <bits/stdc++.h>
using namespace std;
//图的递归栈 
/*
vector<vector<int>> adj={{1,2},{0,3},{0},{1}};
vector<bool> visited (adj.size(),false);
void dfs(int u)
{
    cout<<"进入节点："<<u<<endl;
    visited[u]=true;

    for(int v:adj[u])
    {
        if(!visited[u])
        dfs(v);
    }
    cout<<"离开节点："<<u<<endl;
}
int main()
{
    dfs(0);
    return 0;
}
*/
//图的BFS 队列拓展
vector<vector<int>> adj={{1,2},{0,4,8},{2,8,5},{4,8,6},{5,7,8},{3,8,6},{7,1},{0,3},{1,2,5,7}};
void bfs(int start)
{
    vector<bool>visited (9,false);
    queue<int>q;
    q.push(start);
    visited[start]=true;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        cout<<"访问节点："<<u<<endl;
        for(int v:adj[u])
        {
            if(!visited[v])
            {
               visited[v]=true;
               q.push(v);
            }
        }
    }
}
int main()
{
    bfs(0);
    return 0;
}