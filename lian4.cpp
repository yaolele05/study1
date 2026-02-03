#include <bits/stdc++.h>
using namespace std;
/*
int main()
{
   ios::sync_with_stdio(false);
   cin.tie(nullptr);
   int m,k;
   cin>>m>>k;
   list<int>lst;
   unordered_map<int,list<int>::iterator> pos;
   for(int i=0;i<m;i++)
   {
    int x;
    cin>>x;
    //不存在
    if(pos.find(x)==pos.end())
    {
        if(lst.size()==k)
        {
            int old=lst.back();
            pos.erase(old);
            lst.pop_back();

        }
        lst.push_front(x);
        pos[x]=lst.begin();
    }
    else//存在
    {
        lst.erase(pos[x]);
        lst.push_front(x);
        pos[x]=lst.begin();


    }
   }
   for(int x:lst)
   {
    cout<<x<<' ';
   
   }
    cout<<endl;
    return 0;
}
*/
/*
给定长度为 n 的数组 a，窗口大小为 k。
对每个长度为 k 的窗口，输出当前窗口内不同元素的数量。
典型解法：滑动窗口 + 哈希表计数。
*/
#include <bits/stdc++.h>
using namespace std;
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,k;//k:窗口大小，n:数组长度
    cin>>n>>k;
   vector<int>a(n);//数组
   for(int i=0;i<k;i++)
   {
    cin>>a[i];
   }
   unordered_map<int,int>ci;//哈希表，记录窗口内每个元素出现的个数
   vector<int> result;//保存每个窗口中不同元素的个数
   for(int i=0;i<k;i++)
   {
    ci[a[i]]++;
   }
   result.push_back(ci.size());//第一个窗口的不同元素的个数
   //滑动窗口，每次右移一个位置
   for(int i=k;i<n;i++)
   {
    //移出窗口最左边的元素
    int left=a[i-k];
    ci[left]--;
    if(ci[left]==0)
    ci.erase(left);
    //新元素加入窗口
    int right=a[i];
    ci[right]++;
    result.push_back(ci.size());
   }
   for(int x:result)
   {
    cout<<x<<" ";
   }
   cout<<endl;
    return 0;
}
