#include <bits/stdc++.h>
using namespace std;
/*
int main()
{
    int n;
    cin>>n;
    vector<int>a(n);
    for(int i=0;i<n;i++)
    {
        int x;
        cin>>x;
        a[x]++;
    }
    int j;
    for(j=1;j<n;j++)
    {
        while(a[j]--)
            cout<<j<<" ";
        
    }
    cout<<j;
     cout<<j+1<<endl;
    
    return 0;
}
//分治
int part(vector<int>&a,int l,int r )
{
    int ave=a[l];
    int p=l;
    for(int i=l+1;i<=r;i++)
    {
        if(a[i]<=ave)
        {
            p++;
            swap(a[p],a[i]);
        }
    }
    swap(a[l],a[p]);
    return p;
}
int qselect(vector<int>&a,int k)
{
  int l=0,r=a.size()-1;
  while(l<=r)
  {
    int pos=part(a,l,r);
    if(pos==k) return a[pos];
    if(pos>k) r=pos-1;
    else
    l=pos+1;
  }
  return -1;
}
int main()
{
    int n,k;
    cin>>n,k;
    vector<int>a(n);
    for(int i=0;i<n;i++)
    cin>>a[i];
    cout<<qselect(a,k)<<endl;
    return 0;
}
*/

#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    vector<int>a;
    unordered_set<int> exist;
    for(int i=0;i<n;i++)
    {
        int x;
        cin>>x;
        if(exist.count(x)!=0)
        {
            auto it=find(a.begin(),a.end(),x);
            a.erase(it);
         }
        else
        {
            exist.insert(x);
       }
        a.push_back(x);
        
        
     }
    sort(a.begin(),a.end());
    cout<<a.size()<<endl;
    for(int i:a)
    {
        cout<<i<<" ";
    }
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

struct Student
{
    int id;     // 学号
    int chi;    // 语文成绩
    int sum;    // 总分
};

bool cmp(const Student &a, const Student &b)
{
    if (a.sum != b.sum)
        return a.sum > b.sum;   // 总分降序
    if (a.chi != b.chi)
        return a.chi > b.chi;   // 语文降序
    return a.id < b.id;         // 学号升序
}

int main()
{
    int n;
    cin >> n;

    vector<Student> v;

    for (int i = 1; i <= n; i++)
    {
        int c, m, e;
        cin >> c >> m >> e;
        Student s;
        s.id = i;
        s.chi = c;
        s.sum = c + m + e;
        v.push_back(s);
    }

    sort(v.begin(), v.end(), cmp);

    for (int i = 0; i < 5; i++)
    {
        cout << v[i].id << " " << v[i].sum << endl;
    }

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n;
    cin >> n;

    string maxVotes = "";
    int winner = 1;

    for (int i = 1; i <= n; i++)
    {
        string votes;
        cin >> votes;

        if (maxVotes == "" || 
            votes.size() > maxVotes.size() || 
           (votes.size() == maxVotes.size() && votes > maxVotes))
        {
            maxVotes = votes;
            winner = i;
        }
    }

    cout << winner << endl;
    cout << maxVotes << endl;

    return 0;
}
