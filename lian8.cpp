/*#include <bits/stdc++.h>
using namespace std;
int main()
{
    int n;
    cin>>n;
    string maxs;
    int win;
     for(int i=1;i<=n;i++)
     {
        string m;
        cin>>m;
        if(i==1)
        {
            maxs=m;
            win=1;
        }
        else
        {
            if(m.size()>maxs.size()||m.size()==maxs.size()&&m>maxs)
            {
                maxs=m;
                win=i;
            }
        }
     }
         cout<<win<<"\n";
         cout<<maxs<<endl;
    return 0;
}*/
//动态规划思想与分治策略
#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using boost::multiprecision::cpp_int;