#include <bits/stdc++.h>
using namespace std;
/*去重
int main()
{
   deque<int>q;
   unordered_set<int> exist;
   int m;
   int n;
   cin>>m>>n;
   for(int i=0;i<m;i++)
   {
    int x;
    cin>>x;
     if(exist.count(x)==0)
     {
        if(exist.size()>=n)
        { int old=q.back();
            exist.erase(old);
            q.pop_back();
        }
        q.push_front(x);
        exist.insert(x);
    
     }
     else
     {
        auto it=find(q.begin(),q.end(),x);
        q.erase(it);
        q.push_front(x);
     }
   }
     

    return 0;
}
*/
/*
int main()
{
    deque<int>q;
    unordered_set<int> exist;
    int m;
    int n;
    cin>>m>>n;
    for(int i=0;i<m;i++)
    {
        int x;
        cin>>x;
        if(exist.count(x)==0)
        {
          
            if(exist.size()>n)
            {
                int old=q.back();
                exist.erase(old);
                q.pop_back();
            }
            q.push_front(x);
            exist.insert(x);
        }
        else
        {
            auto it=find(q.begin(),q.end(),x);
            q.erase(it);
            q.push_front(x);
        }
    }
    return 0;
}
*/
/*
int main()
{
    unordered_set<int> exist;
    int m,n;
    cin>>m>>n;
    deque<int> q;
    for(int i=0;i<m;i++)
    {
        int x;
        cin>>x;
       if(exist.count(x)==0)
       {
             if(exist.size()>=n)
             {
                int old=q.back();
                 exist.erase(old);
                 q.pop_back();

             }
             q.push_front(x);
            exist.insert(x);             
       }
       else
       {
          auto it=find(q.begin(),q.end(),x);
          q.erase(it);
          q.push_front(x);
       }
    }
    for (int x : q)
    cout << x << " ";
     cout << endl;
}
*/
#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int m,k;//，m是访问次数，k是容量
    cin>>m>>k;
    list<int> lst;
    unordered_map<int,list<int>::iterator> pos;
    for(int i=0;i<m;i++)
    {
        int x;
        cin>>x;
        //不存在
        if(pos.find(x)==pos.end())
        {
            if((int)lst.size()==k)
            {
                int old=lst.back();
                lst.pop_back();
                pos.erase(old);
            }
            lst.push_front(x);
            pos[x]=lst.begin();

        }
        //已存在
        else
        {
            lst.erase(pos[x]);
            lst.push_front(x);
            pos[x]=lst.begin();
        }
    }

    for(int x:lst)
    {
        cout<<x<<" ";
        cout<<'\n';
    }
    return 0;
}
