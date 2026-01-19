#include <iostream>
using namespace std;
struct Node
{
    int data;
    Node*next;
    //构造函数
    Node(int val):data(val),next(nullptr){}

};
class LinkedList
{
    public:
    Node*head;
    LinkedList():head(nullptr){}
    //成员函数
    void append(int a)
    {
        Node* newNode=new Node(a);
        if(head==nullptr)
        {
            head=newNode;
            return;

        }
        Node*t=head;
        while(t->next!=nullptr)
        {
            t=t->next;
        }
        t->next=newNode;

    }
    void print()
    {
        Node*t=head;
        while(t!=nullptr)
        {
            cout<<t->data<<"->";
            t=t->next;

        }
        cout<<"nullptr"<<endl;
    }
    ~LinkedList()
    {
        Node*t=head;
        while(t!=nullptr)
        {
            Node*nextN=t->next;
            delete t;
            t=nextN;
        }
    }
};
int main()
{
    LinkedList list;
    list.append(1);
    list.append(2);
    list.append(3);
    list.print();
    return 0;
}
 /*: 与 :: 的区别总结
运算符	                             用途	                             示例
:                    类成员初始化列表：构造函数初始化成员变量	   MyClass(int x, int y) : a(x), b(y) {}
                     继承关系：表示类的继承结构	                    class Derived : public Base {}
                      类成员作用域：在类外定义成员函数时，指定其属于哪个类	 void MyClass::print() {}
::	                   作用域解析：指定成员属于哪个类、命名空间或全局作用域	  MyClass::count, ::value
                            访问类的静态成员	                            MyClass::count = 10;
                          访问命名空间中的成员	                          MyNamespace::value
*/