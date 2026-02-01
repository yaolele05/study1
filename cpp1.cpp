/*
1️⃣ 成员对象是“内嵌对象”，不是指针
2️⃣ 成员对象一定比外层对象先构造
3️⃣ 成员对象只能用初始化列表构造
*/
/*
class 默认 private
struct 默认 public
*/
#include <iostream>
using namespace std;
class A
{
    int a;
};
//等价于
struct B
{
    private:
    int a;
};

//构造函数：没有返回值 创建对象时自动调用
struct Node
{
    public:
    int data;
    Node* next;
    Node(int a):data(a),next(nullptr){};
};
int main()
{
    Node*p=new Node(12);
    cout<<p->data<<endl;
    delete p;
    return 0;
}