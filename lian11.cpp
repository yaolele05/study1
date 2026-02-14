#include <bits/stdc++.h>
using namespace std;
/*int main()
{
    string s;
    cin>>s;
    int maxl=0;
    int left=0;
    vector<int> last(128,-1);
    for(int right=0;right<s.size();right++)
    {
       if(last[s[right]]>=left)
       {
         left=last[s[right]]+1;
       }
       last[s[right]]=right;
       maxl=max(maxl,right-left+1);
    }
    return 0;
}
*/
/*
int main()
{
    string s,p;
    cin>>s>>p;
    vector<int>result;
   if(s.size()<p.size())
   return 0;
   vector<int> need(26,0);
   vector<int> win(26,0);
   for(char c:p)
   {
    need[c-'a']++;
   }
   int left=0;
   for(int right=0;right<s.size();right++)
   {
                 win[s[right]-'a']++;//加入窗口元素
                 //保持窗口大小
                 if(right-left+1>p.size())
                 {
                    win[s[left]-'a']--;
                    left++;
                 }
                 if(right-left+1==p.size())
                 {
                    if(win==need)
                    result.push_back(left);
                 }
   }


    return 0;
}*/
//前缀和 + 哈希表
/*
int main()
{
    int n;
    cin>>n;
    vector<int> nums(n);
    for(int i=0;i<n;i++)
    {
      cin>>nums[i];
    }
    int k;
    cin>>k;
    unordered_map<int,int> f;
    f[0]=1;
    int sum=0;
    int count=0;
    for(int x:nums)
    {
        sum+=x;
        if(f.count(sum-k));
        count+=f[sum-k];
        f[sum]++;
    }
    return 0;
}
*/
/*
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    vector<int> result;
    deque<int> dq; // 存储元素索引的双端队列

    for (int i = 0; i < nums.size(); ++i) {
        // 移除队列中不在当前窗口内的元素
        if (!dq.empty() && dq.front() < i - k + 1) {
            dq.pop_front();
        }

        // 移除队列中小于当前元素的元素，因为它们不可能是当前窗口的最大值
        while (!dq.empty() && nums[dq.back()] < nums[i]) {
            dq.pop_back();
        }

        // 将当前元素索引加入队列
        dq.push_back(i);

        // 当窗口形成时，记录当前窗口的最大值
        if (i >= k - 1) {
            result.push_back(nums[dq.front()]);
        }
    }

    return result;
}

int main() {
    vector<int> nums = {1,3,-1,-3,5,3,6,7};
    int k = 3;
    vector<int> result = maxSlidingWindow(nums, k);
    
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}

*/