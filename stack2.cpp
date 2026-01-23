#include <iostream>
using namespace std;
//链表实现栈，加一些功能
struct Node
{
    int data;
    Node*next;
    Node(int a):data(a),next(nullptr){}
};
class Stack
{
    private:
    Node*top;
    public:
    Stack():top(nullptr){}
    //入栈
    void push(int b)
    {
        Node*node=new Node(b);
        node->next=top;
        top=node;

    }
    //出栈
    int pop()
    {
      if(top==nullptr)
      {
        cout<<"none"<<endl;
        return-1;
      }
      Node*t=top;
      int c=t->data;
      top=top->next;
      delete t;
      return c;

    }
    bool empty()
    {
        return top==nullptr;
    }
    //析构函数
    ~Stack()
   {
    while(top)
    {
        Node* t = top;
        top = top->next;
        delete t;
    }
 }
    //查看栈顶元素
    int peek()
    {
        if(top==nullptr)
        {
            cout<<"none"<<endl;
            return -1;
        }
        return top->data;
    }
    //返回栈中元素个数
    int size()
    {
        int count=0;
        Node*t =top;
        while(t!=nullptr)
        {
            count++;
            t=t->next;
        }
        return count;
    }
};
int main()
{
    Stack s;
    int a,b,c,d;
    cin>>a>>b>>c>>d;
    s.push(a);
    s.push(b);
    s.push(c);
    s.push(d);
    cout<<s.size()<<endl;
    cout<<s.peek()<<endl;
    while(s.empty()==false)
    cout<<s.pop()<<" ";
    cout<<endl;
    return 0;

}