#include <iostream>
using namespace std;

struct Node
{
    int data;
    Node* next;
    
    // 构造函数
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList
{
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    // 添加节点
    void append(int a)
    {
        Node* newNode = new Node(a);// new 会 自动调用构造函数
        if (head == nullptr)
        {
            head = newNode;
            return;
        }
        Node* t = head;
        while (t->next != nullptr)
        {
            t = t->next;
        }
        t->next = newNode;
    }

    // 插入节点
    void insertAt(int position, int value)
    {
        if (position < 0) return;  
        Node* newNode = new Node(value);
        if (position == 0)
        {
            newNode->next = head;
            head = newNode;
            return;
        }

        Node* t = head;
        for (int i = 1; t != nullptr && i < position; i++)
        {
            t = t->next;
        }

        if (t == nullptr)
        {
            cout << "Position out of bounds!" << endl;
            delete newNode;
            return;
        }

        newNode->next = t->next;
        t->next = newNode;
    }

    // 删除节点
    void deleteNode(int value)
    {
        Node* t = head;
        if (t != nullptr && t->data == value)
        {
            head = t->next;
            delete t;
            return;
        }

        Node* prev = nullptr;
        while (t != nullptr && t->data != value)
        {
            prev = t;
            t = t->next;
        }

        if (t == nullptr)
        {
            cout << "Value not found!" << endl;
            return;
        }

        prev->next = t->next;
        delete t;
    }

    // 查找值
    bool contains(int value)
    {
        Node* t = head;
        while (t != nullptr)
        {
            if (t->data == value)
                return true;
            t = t->next;
        }
        return false;
    }

    // 反转链表
    void reverse()
    {
        Node* prev = nullptr;
        Node* curr = head;
        Node* next = nullptr;
        
        while (curr != nullptr)
        {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
    }

    // 链表长度
    int length()
    {
        int len = 0;
        Node* t = head;
        while (t != nullptr)
        {
            len++;
            t = t->next;
        }
        return len;
    }

    // 打印链表
    void print()
    {
        Node* t = head;
        while (t != nullptr)
        {
            cout << t->data << "->";
            t = t->next;
        }
        cout << "nullptr" << endl;
    }

   
    ~LinkedList()
    {
        Node* t = head;
        while (t != nullptr)
        {
            Node* nextN = t->next;
            delete t;
            t = nextN;
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
    list.insertAt(1, 4);  
    list.print();  

    list.deleteNode(2);  
    list.print(); 

    cout << "Contains 3: " << (list.contains(3) ? "Yes" : "No") << endl;  

    list.reverse();  
    list.print(); 

    cout << "Length: " << list.length() << endl;  

    return 0;
}
