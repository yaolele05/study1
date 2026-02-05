//排序 + 二分 + 模拟
//归并排序
//左半区（有序）：[4, 6, 9, 12]
//右半区（有序）：[3, 10]
//因为左边是有序的，所以：一下子产生 4 个逆序对：mid - i + 1
#include <bits/stdc++.h>
using namespace std;
 long long cnt=0;
 vector<int> t;
 void msort(vector<int>&a,int l,int r)
 {
    if(l>=r)
    return;
    int mid=(l+r)/2;
    msort(a,l,mid);
    msort(a,mid+1,r);
    int i=l,j=mid+1,k=l;
    while(i<=mid&&j<=r)
    {
        if(a[i]<=a[j])
        t[k++]=a[i++];
        else
        {
            t[k++]=a[j++];
            cnt+=mid-i+1;//逆序对
        }
    }
   while(i<=mid)
   t[k++]=a[i++];
   while(j<=r)
   t[k++]=a[j++];
   for(int x=l;x<=r;x++)
   a[x]=t[x];
 }

/*
堆排序 / priority_queue（重点）
核心
动态维护最大 / 最小
常用写法（B 组推荐）
priority_queue<int> maxHeap;
priority_queue<int, vector<int>, greater<int>> minHeap;
priority_queue 默认是 大根堆
 greater<int> → 小根堆
 top() 只是看，不删除
 pop() 才是真的删
*/
priority_queue<int,vector<int>,greater<int>>q;
void sort()
{
    while(q.size()>1)
    {
        int a=q.top();q.pop();
        int b=q.top();q.pop();
        q.push(a+b);

    }
}
/*
桶排序 / 计数排序（蓝桥杯神器）
使用场景
数据范围小
字符 / 数字频率统计
*/
int cnt[10001]={0};
for(int x:a) cnt[x]++;
int main()
{
    int n,x;
    cin>>n>>x;
    int l=0,r=n-1;
    while(l<r)
    {
        
        int mid=(l+r)/2;
        if(a[mid]>=x)
        r=mid;
        else
        l=mid+1;
    }
    return 0;
}
