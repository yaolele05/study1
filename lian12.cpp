#include <bits/stdc++.h>
using namespace std;
/*
int main()
{
    vector<int> m={-2,1,-3,4,-1,2,1,-5,4};
    vector<int>sum(m.size());
    int maxc=m[0];
     sum[0]=m[0];
    for(int i=1;i<m.size();i++)
    {
       sum[i]=sum[i-1]+m[i];
    }
    for(int i=0;i<m.size();i++)
    {
        for(int j=i;j<m.size();j++)
        {
           int cur=0;
           if(i==0)
           cur=sum[j];
           else
           cur=sum[j]-sum[i-1];
           maxc=max(maxc,cur);
        }
    }
   cout<<maxc<<endl;

    return 0;
}
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        vector<int> sum(nums.size());
        sum[0]=nums[0];
       int maxc=nums[0];
        for(int i=1;i<nums.size();i++)
        {
            sum[i]=sum[i-1]+nums[i];
        }
        for(int i=0;i<nums.size();i++)
        {
            for(int j=i;j<nums.size();j++)
            {
                int cur;
                if(i==0)
                cur=sum[j];
                else
                cur=sum[j]-sum[i-1];
                maxc=max(maxc,cur);
            }
        }
        return maxc;
    }
};*/
/*
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int cur=nums[0];
        int maxs=nums[0];
     for(int i=0;i<nums.size();i++)
     {
      cur=max(nums[i],cur+nums[i]);
      maxs=max(maxs,cur);
     }
     cout<<maxs<<endl;
    }
};
*/
/*
*/
/*
class solution
{
    public:
    vector<vector<int>>merge(vector<vector<int>>&ini)
    {
        vector<vector<int>>result;
        sort(ini.begin(),ini.end());
        for(auto& inte:ini)
        {
            if(result.empty()||result.back()[1]<inte[0])
            result.push_back(inte);
            else
            result.back()[1]=max(result.back()[1],inte[1]);
        }
        return result;
    }
}
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        int n = nums.size();
        k %= n;

        vector<int> temp(n);

        for (int i = 0; i < n; i++) {
            temp[(i + k) % n] = nums[i];
        }

        nums = temp;
*/
class solution
{
    public:
    void rotate(vector<int>&nums,int k)
    {
        int n=nums.size();
        k%=n;
        vector<int>t(n);
        for(int i=0;i<n;i++)
        {
            t[(i+k)%n]=nums[i];
        }
        nums=t;
    }
}