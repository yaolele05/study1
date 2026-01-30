
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
    int n1, n2;
    cin >> n1 >> n2;

    int m;
    cin >> m;

    vector<int> v(m);
    for(int i = 0; i < m; i++)
        cin >> v[i];

    list<int> q1, q2;
    unordered_map<int, int> pos;

    for(int i = 0; i < m; i++)
    {
        int p = v[i];

        if(pos[p] == 0)
        {
            q2.push_front(p);
            pos[p] = 2;

            if(q2.size() > n2)
            {
                int old = q2.back();
                q2.pop_back();
                pos[old] = 0;
            }
        }
        else
        {
            if(pos[p] == 1)
                q1.remove(p);
            else
                q2.remove(p);

            q1.push_front(p);
            pos[p] = 1;

            if(q1.size() > n1)
            {
                int old = q1.back();
                q1.pop_back();

                if(q2.size() < n2)
                {
                    q2.push_front(old);
                    pos[old] = 2;
                }
                else
                {
                    pos[old] = 0;
                }
            }
        }
    }

    for(int x : q1) cout << x << " ";
    cout << endl;
    for(int x : q2) cout << x << " ";
    cout << endl;

    return 0;
}
