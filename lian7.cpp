#include <bits/stdc++.h>
using namespace std;
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