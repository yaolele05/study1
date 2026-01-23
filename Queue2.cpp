#include <iostream>
using namespace std;
//链表实现队列的核心是插入队尾，删除队首
struct Node
{
    int data;
    Node*next;
    Node(int a):data(a),next(nullptr){}
};
class queue
{
    private:
    Node*front;
    Node*rear;
    public:
    queue():front(nullptr),rear(nullptr){}
    void enqueue(int a)
    {
        Node *node=new Node(a);
        if(rear==nullptr)
        {
            front=rear=node;;
            return;
        }
        rear->next=node;
        rear=node;
    }
    void dequeue()
    {
        if(front==nullptr)
        {
            cout<<"none"<<endl;
            return ;
        }
        Node*t=front;
        front=front->next;
        if(front==nullptr)
        {
            rear=nullptr;
        }
        delete t;
    }
    bool empty()
    {
        return front==nullptr;
    }
};

int main() {
    int i = 1;
    do {
        cout << i << " ";
        i++;
    } while (i <= 5);
    return 0;
}