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
/*
1.智能指针

std::unique_ptr：它是独占所有权的智能指针，使用std::move转移所有权。当unique_ptr超出作用域时，它会自动销毁其所管理的资源，避免了内存泄漏。

std::shared_ptr：用于多对象共享所有权，当最后一个shared_ptr被销毁时，才释放资源。

std::weak_ptr：用于打破循环引用，它允许你在shared_ptr管理的对象不再被所有者持有时，避免内存泄漏。
*/
std::unique_ptr<int> ptr1 = std::make_unique<int>(10);  // unique_ptr
std::shared_ptr<int> ptr2 = std::make_shared<int>(20);  // shared_ptr
std::weak_ptr<int> weakPtr = ptr2;  // weak_ptr
//裸指针的问题
int* p = new int(10);
// 中途 return / throw / 忘了 delete
//问题：忘记 delete → 内存泄漏 delete 多次 → 未定义行为 delete
//shared_ptr：共享所有权
//1.隐性代价2.原子引用计数3.内存多一块 control block4.生命周期不清晰
//weak_ptr：打破循环

//2. Lambda表达式和函数对象
std::vector<int> nums = {1, 2, 3, 4, 5};
std::for_each(nums.begin(), nums.end(), [](int n) { std::cout << n << " "; });
//3.模板类：允许为不同类型的数据写一个通用的类
template <typename T>
class Box {
private:
    T value;
public:
    Box(T v) : value(v) {}
    T getValue() { return value; }
};

//unique_ptr
#include <iostream>
#include <memory>  // 引入 unique_ptr

class MyClass {
public:
    MyClass() {
        std::cout << "MyClass constructor" << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destructor" << std::endl;
    }
    void greet() {
        std::cout << "Hello from MyClass!" << std::endl;
    }
};

int main() {
    // 创建 unique_ptr
    std::unique_ptr<MyClass> ptr1 = std::make_unique<MyClass>();

    // 使用对象
    ptr1->greet();

    // ptr1 超出作用域自动调用析构函数
    return 0;
}
//shared_ptr
#include <iostream>
#include <memory>  // 引入 shared_ptr

class MyClass {
public:
    MyClass() {
        std::cout << "MyClass constructor" << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destructor" << std::endl;
    }
    void greet() {
        std::cout << "Hello from MyClass!" << std::endl;
    }
};

int main() {
    // 创建 shared_ptr
    std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>();

    {
        // ptr2 和 ptr1 共享 MyClass 对象
        std::shared_ptr<MyClass> ptr2 = ptr1;
        ptr2->greet();
        std::cout << "ptr2 use count: " << ptr2.use_count() << std::endl;
    }

    // ptr2 超出作用域被销毁，ptr1 仍然拥有对象
    std::cout << "ptr1 use count: " << ptr1.use_count() << std::endl;

    // ptr1 超出作用域，MyClass 被销毁
    return 0;
}
