//STL = 一套“现成的数据结构 + 算法”的标准库
//STL的组件：容器、算法、迭代器
//容器：用来存储数据的
//算法：用来处理数据的
//迭代器：用来连接容器和算法的
//vector:动态数组
//string:字符串
//list:双向链表
//deque:双端队列
//set:集合
//map:  键值对
//algorithm:排序、查找、变换、合并、统计等算法

//注意  cin >> 读到空格就停了
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
//vector:动态数组
int main()
{
    vector<int>arr;//声明一个能存 int 的动态数组 arr
    arr.push_back(10);//在数组末尾添加一个元素 10(尾插法)
    arr.push_back(20);
    arr.push_back(30);
    arr.pop_back();//删除数组末尾的一个元素(尾删法)
    for(int i=0;i<arr.size();i++)
    {
        cout<<arr[i]<<" ";
    }
    return 0;
}
/*
arr.push_back(x)	尾插
arr.pop_back()	尾删
arr[i]	像数组一样访问
arr.size()	元素个数
arr.empty()	是否为空
arr.clear()	清空
*/
int main()
{
    string s="hello";
    s+=" world";//字符串拼接
    cout<<s<<endl;
}
/*
s.size()	长度
s + s2	拼接
s == s2	比较
s[i]	访问字符
*/
int main()
{
    vector<int>arr1={4,5,8,2,1};
    sort(arr1.begin(),arr1.end());//排序    v.end()最后一个元素的下一个
    for(int x:arr1)
    {
        cout<<x<<" ";

    }
    if(find(arr1.begin(),arr1.end(),3)!=arr1.end())
    {
        cout<<"found"<<endl;
    }
    for(auto it=arr1.begin();it!=arr1.end();)
    {
        if(*it==3)
        it = arr1.erase(it);//“删完之后，请把新的、有效的位置给我”
        else
        ++it;//因为it++ 只有在“必须用旧值”时才用
    }
    return 0;

}
//红黑树
/*红黑树 = 二叉搜索树 + 颜色约束
核心思想：通过颜色约束来保证树的平衡性
每个节点要么是红色，要么是黑色
根节点是黑色
每个叶子节点（NIL节点）是黑色
如果一个节点是红色的，则它的两个子节点必须是黑色的（不能有两个连续的红色节点）
从任一节点到其每个叶子节点的所有路径都包含相同数目的黑色节点
*/
/*红黑树维持平衡两个核心操作
 变色（recolor）
 旋转（rotate）
 */
#include <iostream>
#include <map>
#include <string>
using namespace std;
//重点 std::map = 自动排序的“键 → 值”容器（红黑树）
//key  →  value（key 永远有序。）
//map的每一个元素都是一个 pair（键值对）
//map<string,int>m; //key 是 string，value 是 int
int main()
{
    map<string,int>m;
    m["apple"]=5;
    m["banana"]=3;
    auto it=m.find("apple");
    if(it!=m.end())
    cout<<it->first<<" "<<it->second<<endl;
    for(const auto&p:m)//range-for
    {
        cout<<p.first<<" "<<p.second<<endl;
    }
    /*等价于上一个循环
    for (auto it = score.begin(); it != score.end(); ++it)
{
    const auto& p = *it;
    cout << p.first << " " << p.second << endl;
}

    */
    return 0;
}