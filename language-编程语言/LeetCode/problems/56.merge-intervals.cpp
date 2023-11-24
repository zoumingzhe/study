bool compare(vector<int> a, vector<int> b) 
{ 
    return a[0] < b[0];
} 

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(),intervals.end());

        vector<vector<int>> ret;
        int n = intervals.size();

        ret.push_back(intervals[0]);
        for (int i = 1; i < n; i++) {
            if (intervals[i][0] > ret.back()[1]) {
                ret.push_back(intervals[i]);
                continue;
            }

            if (intervals[i][0] < ret.back()[0]) {
                ret.back()[0] = intervals[i][0];
            }

            if (intervals[i][1] > ret.back()[1]) {
                ret.back()[1] = intervals[i][1];
            }
        }
        
        return ret;
    }
};
