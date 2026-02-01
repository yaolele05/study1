#include <bits/stdc++.h>
/*
vector / deque 基本操作
模拟队列
模拟栈
模拟滑动窗口（deque）

*/
/*
关键词：
push_back / push_front
pop_back / pop_front
front / back
*/
/*
模型 1：LRU / MRU / FIFO 模拟

单队列 LRU

双队列冷热分离（你这题）

带容量限制的访问记录

“需要的是 顺序，还是 存在，还是 映射关系？”
*/
/*
模型 2：频率 + 最近性混合

例如：

访问次数最多

最近访问优先

访问次数相同按时间排序

是 map + set / priority_queue 的主战场
*/

/*
unordered_map<K, V> 的两个参数：
K：我用什么来“查”
V：我查到之后，真正需要的“东西”是什么
unordered_map<int, list<int>::iterator> pos;   :: —— 作用域解析运算符
给我一个数据页编号，我立刻知道它在 list 里的位置
因为这个 map 的含义是：
元素 → 在 list 里的位置
unordered_map<...>：类型
pos：变量名
: —— 不是一个东西，而是三种不同语法
（1）构造函数初始化列表（2）继承（3）条件运算符（三目运算）（4）范围 for（语法糖）
*/


/*
① LRU 缓存:
1.按“最近使用”排序2.访问到已有元素 → 必须立刻删掉旧位置3.不能 O(n) 查找
*/
#include <bits/stdc++.h>
using namespace std;
/*
int main()
{
    int k;//缓存容量
    int m;//访问次数

    cin>>k>>m;
    deque<int> q;
    unordered_set <int> exist;
    for(int i=0;i<m;i++)
    {
        int x;    
        cin>>x;
        if(exist.count(x)==0)
        {
            if(q.size()==k)
            {
              int old=q.back();
              q.pop_back();
              exist.erase(old);
            }
            q.push_front(x);
            exist.insert(x);
        }
        else
        {
           auto it =find(q.begin(),q.end(),x);
           if(it!=q.end())
            q.erase(it);
            q.push_front(x);
        }
    }

    return 0;
}
 
int main()
{
    deque <int>q;
    int m;
    cin>>m;
    unordered_set<int> exist;
    for(int i=0;i<m;i++)
    {
        int x;
        cin>>x;
        if(exist.count(x)==0)
        {
            q.push_front(x);
            exist.insert(x);
        }
        else
        {
            auto it=find(q.begin(),q.end(),x);
            if(it!=q.end())
            q.erase(it);
            q.push_front(x);
        }
    }
  for(int x:q)
  {
    cout<<x<<" ";
  }
    return 0;
}
*/
int main()
{
    deque<int>q;
    int k;
    cin>>k;
    unordered_set<int> exist;
    for(int i=0;i<k;i++)
    {
        int x;
        cin>>x;
        if(exist.count(x)==0)
        {
            if(exist.size()>=k)
            {
                int old=q.back();
              exist.erase(old);
              q.pop_back();
              q.push_front(x);
              exist.insert(x);   
            }
            else
            {
                q.push_front(x);
                exist.insert(x);
            }
        }
        else
        {
            
                auto it=find(q.begin(),q.end(),x);
                if(it!=q.end())
                q.erase(it);
            q.push_front(x);
            
        }
    }
    return 0;
}