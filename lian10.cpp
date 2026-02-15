#include <iostream>
using namespace std;
/*class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> m;
        for(int i=0;i<nums.size();i++)
        {
            int need=target-nums[i];
            if(m.count(need))
            return {m[need],i};
            else
            m[nums[i]]=i;
        }
         return {};
    }
    
};
*/
/*
class solution
{
public:
   vector<vector<string>>ga(vector<string>&strs)
{
   unordered_map<sting,vector<string>>mp;
   for(string s:strs)
   {
    string key=s;
    sort(key.begin(),key.end());
    mp[key].push_back(s);
   }
  vector<vector<string>>result;
   for(auto pair:mp)
   {
    result.push_back(pair.secomd);
    }
    return result;
}
}
*/
/*
int main()
{
    string s;
    cin>>s;
    int len=1;
    int mlen=1;
        for(int i=1;i<s.size()-1;i++)
        {
             
            if(s[i-1]==s[i]-1)
            len++;
            else
            len=1;

                    mlen=max(len,mlen);
      
        }
       cout<<mlen<<endl;
        return 0;
 }
*/
/*class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> res;
        if (s.size() < p.size()) return res;

        vector<int> need(26, 0);
        vector<int> window(26, 0);

        for (char c : p) {
            need[c - 'a']++;
        }

        int left = 0;
        int right = 0;
        int valid = 0;  // 有多少字符频率匹配

        while (right < s.size()) {
            int idx = s[right] - 'a';
            window[idx]++;
            
            if (window[idx] == need[idx]) {
                valid++;
            }

            right++;

            // 当窗口大小达到 p.size()
            while (right - left >= p.size()) {

                if (valid == 26) {  
                    // 这个写法不严谨，下面给更优版本
                }

                if (right - left == p.size()) {
                    if (window == need) {
                        res.push_back(left);
                    }
                }

                int lidx = s[left] - 'a';
                window[lidx]--;
                left++;
            }
        }

        return res;
    }
};*/
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> res;
        if (s.size() < p.size()) return res;

        vector<int> need(26, 0), window(26, 0);

        for (char c : p) {
            need[c - 'a']++;
        }

        int left = 0;

        for (int right = 0; right < s.size(); right++) {
            window[s[right] - 'a']++;

            // 保证窗口大小等于 p.size()
            if (right - left + 1 > p.size()) {
                window[s[left] - 'a']--;
                left++;
            }

            if (right - left + 1 == p.size()) {
                if (window == need) {
                    res.push_back(left);
                }
            }
        }

        return res;
    }
};
