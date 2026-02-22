 sort(nums.begin(), nums.end());
        int n = nums.size();
        
        int closest = nums[0] + nums[1] + nums[2]; // 初始化
        
        for (int i = 0; i < n - 2; i++) {
            int left = i + 1;
            int right = n - 1;
            
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                
                // 更新最接近的值
                if (abs(sum - target) < abs(closest - target)) {
                    closest = sum;
                }
                
                if (sum < target) {
                    left++;      // 让和更大
                } else if (sum > target) {
                    right--;     // 让和更小
                } else {
                    return sum;  // 完全相等，最优
                }
            }
        }
        
        return closest;