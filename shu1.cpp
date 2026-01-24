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