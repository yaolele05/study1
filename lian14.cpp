#include<bits/stdc++.h>
using namespace std;
//树DFS
/*int dfs(TreeNode* root) {
    if (!root) return 0;

    int left = dfs(root->left);
    int right = dfs(root->right);

    // 在这里利用左右子树结果
    return left + right + 1;
}
//树BFS
#include <queue>

void bfs(TreeNode* root) {
    if (!root) return;

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* cur = q.front();
        q.pop();

        cout << cur->val << " ";

        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
}
//图BFS
void bfs(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int cur = q.front(); q.pop();

        for (int next : graph[cur]) {
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }
}
*/
//二叉搜索树插入DFS

struct TreeNode
{
    int val;
    TreeNode*left;
    TreeNode*right;
    TreeNode(int x):val(x),left(nullptr),right(nullptr){};
};
/*
TreeNode* insert(TreeNode*root,int val)
{
    if(!root) return new TreeNode(val);
     if(val<root->val)
     {
        root->left=insert(root->left,val);

     }  
     else if(val>root->val)
     {
        root->right=insert(root->right,val);
        return root;
     } 
}
*/
/*
class solution
{
    public:
    int maxD(TreeNode*root)
    {
        if(!root) return 0;
        queue<TreeNode*>q;
        q.push(root);
        int dep=0;
        while(!q.empty())
        {
            int sz=q.size();
            for(int i=0;i<sz;i++)
            {
                TreeNode*node=q.front();
                q.pop();
                if(node->left) q.push(node->left);
                if(node->right) q.push(node->right);
            }
            dep++;
        }
        return dep;
    }
};
*/
/*
class solution
{
    public:
    int maxd(TreeNode* root)
    {
        if(!root) return 0;
        int left=maxd(root->left);
        int right=maxd(root->right);
        return max(left,right)+1;
    }

}
*/
//图DFS
void dfs(vector<vector<char>>&g,int i,int j)
{
    int m=g.size();
    int n=g[0].size();
    if(i<0||i>=m||j<0||j>n||g[i][j]=='0')
    return ;
    g[i][j]='0';
    dfs(g,i+1,j);
    dfs(g,i-1,j);
    dfs(g,i,j+1);
    dfs(g,i,j-1);
}
int numsl(vector<vector<char>>&g)
{
    int m=g.size();
    int n=g[0].size();
    int count=0;
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(g[i][j]=='1')
            {
                count++;
                dfs(g,i,j);
            }
        }
    }
    return count;
}