//后进先出
//现在先用数组实现一个栈
#include<iostream>
using namespace std;
class stack
{
    private:
    int top;
    int arr[100];
    public:
     stack():top(-1){}
     void push(int a)
     {
         arr[++top]=a;


     }
     int pop()
     {
         if(top==-1)
         {
            cout<<"none"<<endl;
            return -1;
         }
         return arr[top--];
     }
};

//用链表实现栈

struct Node {
    int data;
    Node* next;

    Node(int val) : data(val), next(nullptr) {}
};

class Stack {
private:
    Node* top;

public:
    Stack() : top(nullptr) {}

    void push(int x) {
        Node* node = new Node(x);
        node->next = top;
        top = node;
    }

    int pop() {
        if (top == nullptr) {
            cout << "栈为空" << endl;
            return -1;
        }

        Node* tmp = top;
        int val = tmp->data;
        top = top->next;
        delete tmp;
        return val;
    }

    bool empty() {
        return top == nullptr;
    }
};

int main() {
    Stack s;

    s.push(1);
    s.push(2);
    s.push(3);

    while (s.empty()==false) {
        cout << s.pop() << " ";
    }

    cout << endl;
    return 0;
}
