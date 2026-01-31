
#include <bits/stdc++.h>
using namespace std;
/*int main()
{

    long long sum=0;
    int n;
    cin>>n;
    vector<int> x(n),y(n);
    for(int i=0;i<n;i++)
    {
        cin>>x[i];
    }
    for(int i=0;i<n;i++)
    {
        cin>>y[i];
    }
    sort(x.begin(),x.end());
    sort(y.begin(),y.end());
    for(int i=0;i<n;i++)
    {
        sum+=abs(x[i]-y[i]);
    }
  cout<<sum<<endl;
    return 0;
}
*/
/*
bool check(long long x,vector<vector<int>>& group)
{
    for(int r=0;r<group.size();r++)
    {
        long long shen=0;
        for(int v:group[r])
        {
            if(v>=x)
            {
                shen+=v-x;
            }
            else
            {
               
                long long need=x-v;
                if(shen<need)
                return false;
                 shen-=need;

            }

            }
        }
        return true;
}
    

int main()
{
    int  n,k;
    cin>>n>>k;
    vector<int> a(n);
    
    for(int i=0;i<n;i++)
    {
        cin>>a[i];
    }
    vector<vector<int>> group(k);
    for(int i=0;i<n;i++)
    {
        group[i%k].push_back(a[i]);
    }
    long long max=0;
    for(long long i=0;i<=100000;i++)
    {
        if(check(i,group))
        {
            max=i;
        }
        else
        break;
    }
    cout<<max<<endl;
    return 0;
}
#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n1,n2;
    cin>>n1,n2;
    int m;
    cin>>m;
    vector<int> v(m);
    for(int i=0;i<m;i++)
    {
        cin>>v[i];
    }
    //定义两个队列 + 位置表
    list<int> q1,q2;
    unordered_map<int,int> pos;
    for(int i=0;i<m;i++)
    {
        int p=v[i];
        //p不在队列中
        if(pos[p]==0)
        {
            //插入q2首部
            q2.push_front(p);
            pos[p]=2;
            if(q2.size()>n2)
            {
                int old=q2.back();
                q2.pop_back();
                pos[old]=0;
            }
        }
        //p 已经存在（在 q1 或 q2）
        else
        {
            if(pos[p]==1)
            q1.remove(p);//一定会遍历链表，删除值等于 p 的节点,被删的那个节点内存会被 delete
            else
            q2.remove(p);//是 O(n)
            q1.push_front(p);
            pos[p]=1;
            if(q1.size()>n1)
            {
                int old=q1.back();
                q1.pop_back();
                if(q2.size()<n2)
                {
                    q2.push_front(old);
                    pos[old]=2;
                }
                else
                {
                    pos[old]=0;
                }
            }
        }
    }
    for(int x:q1)
    {
        cout<<x<<" ";

    }
    cout<<endl;
    for(int x:q2)
    {
        cout<<x<<" ";
    }
    cout<<endl;
    return 0;
}
*/
//deque = 双端时间队列
#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n1;
    cin>>n1;
    deque<int>q1;
    unordered_map<int,int>pos;
    unordered_set<int>exist;
    int m;
    cin>>m;
    for(int i=0;i<m;i++)
    {
        int x;
        cin>>x;
        if(exist.count(x)==0)
        {
           if(q1.size()==n1)
           {
            int old=q1.back();
            q1.pop_back();
             exist.erase(old);
           }
           q1.push_front(x);
           exist.insert(x);
        }
        //已存在
        else
        {
            auto it=find(q1.begin(),q1.end(),x);
            q1.erase(it);
            q1.push_front(x);
        }
    }
    for(int x:q1)
    cout<<x<<" ";
    cout<<endl;
   return 0;
}