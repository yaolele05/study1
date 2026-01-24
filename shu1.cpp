//树本身就是递归结构
/*
名称	          含义
根节点	      最上面的节点（A）
父节点	         上一级
子节点	          下一级
叶子节点	没有子节点的节点（D E F）
子树	    某个节点下面的一整棵树
度	         一个节点的子节点个数
*/
//1.二叉树的基本结构,手动创建二叉树
#include <iostream>
using namespace std;
struct Node
{
    int data;
    Node*left;
    Node*right;
    
};
Node*create(int x)
{
    Node*p=new Node();
    p->data=x;
    p->left=nullptr;
    p->right=nullptr;
    return p;
}
//手动创建一棵二叉树
/*int main()
{
    Node*root =create(1);
    root->left=create(2);
    root->right=create(3);
    return 0;

}*/
//2.二叉树的遍历
//前序遍历：根-左-右
void preorder(Node*root)
{
    if(root==nullptr)
    {
        return ;
    }
    cout<<root->data<<" ";
    preorder(root->left);
    preorder(root->right);
}
//中序遍历：左-根-右
void inorder(Node*root)
{
    if(root==nullptr)
    {
        return;
    }
    inorder(root->left);
    cout<<root->data<<" ";
    inorder(root->right);
}
//后序遍历：左-右-根
void postorder(Node*root)
{
    if(root==nullptr)
    {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    cout<<root->data<<" ";  
}
int main()
{
    Node*root=create(1);
    root->left=create(2);
    root->right=create(3);
    root->left->left=create(4);
    root->left->right=create(5);
    root->right->left=create(6);
    root->right->right=create(7);
    preorder(root);
    cout<<endl;
    inorder(root);
    cout<<endl;
    postorder(root);
    cout<<endl;
    return 0;
}
//3.BST的插入
/*
BST:
左子树所有值 < 根节点 < 右子树所有值
并且这个规则 对每一棵子树都成立
*/
//插入本质 = 从根开始，一路比较，走到空指针为止

/*插入过程（配图）

假设我们依次插入：

4, 2, 6, 1, 3, 5, 7

插入 4（第一个节点）
4

插入 2

2 < 4 → 去左边

左边是空 → 插在这里

  4
 /
2

插入 6

6 > 4 → 去右边

右边是空 → 插在这里

  4
 / \
2   6

插入 1

1 < 4 → 左

1 < 2 → 左

空 → 插入

    4
   / \
  2   6
 /
1

插入 3

3 < 4 → 左

3 > 2 → 右

空 → 插入

    4
   / \
  2   6
 / \
1   3

插入 5、7 后最终结果
              4
           /     \
          2       6
        /   \   /   \
       1     3 5     7

*/
//BST的插入
Node*insert(Node*root,int x)
{
    if(root==nullptr)
    return create(x);
    if(x<root->data)
    root->left=insert(root->left,x);
    else if(x>root->data)
    root->right=insert(root->right,x);
    return root;
}
//BST的查找
Node*find(Node*root,int x)
{
    if(root==nullptr)
    return nullptr;
    if(x==root->data)
    return root;
    else if(x<root->data)
    return find(root->left,x);
    else 
    return find(root->right,x);
}
//BST的删除
//1.删除叶子节点（最简单）:直接让父节点指向 nullptr
//2.删除只有一个子节点的节点:用孩子顶替自己
//3.删除有两个孩子的节点（重点）:用右子树最小值(常用）或左子树最大值顶替自己
//BST的删除
Node*findMin(Node*root)
{
    while(root->left!=nullptr)
    {
        root =root->left;
    }
    return root;
}
Node*remove(Node*root,int x)
{
    if(root==nullptr)
    return nullptr;
    if(x<root->data)
    {
        root->left=remove(root->left,x);
    }
    else if(x>root->data)
    {
        root->right=remove(root->right,x);
    }
    else
    {
        //找到节点
          if(root->left==nullptr)//左子树为空
          {
            Node*t=root->right;
            delete root;
            return t;//当前节点被删了，子树根发生变化
          }
           else if(root->right==nullptr)//右子树为空
           {
            Node*t=root->left;
            delete root;
            return t;//当前节点被删了，子树根发生变化
           }
           else//左右子树都不为空
           {
            Node*t=findMin(root->right);
            root->data=t->data;
            root->right=remove(root->right,t->data);//删除右子树最小节点
           
          }
    }
    return root;//当前节点没被删，只是子树内部调整
    }






