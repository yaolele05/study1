//树DFS
int dfs(TreeNode* root) {
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