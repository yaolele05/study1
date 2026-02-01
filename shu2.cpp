//红黑树
/*红黑树 = 二叉搜索树 + 颜色约束
核心思想：通过颜色约束来保证树的平衡性
每个节点要么是红色，要么是黑色
根节点是黑色
每个叶子节点（NIL节点）是黑色
如果一个节点是红色的，则它的两个子节点必须是黑色的（不能有两个连续的红色节点）
从任一节点到其每个叶子节点的所有路径都包含相同数目的黑色节点
*/
/*红黑树维持平衡两个核心操作
 变色（recolor）
 旋转（rotate）
 */
#include <iostream>
#include <map>
#include <string>
using namespace std;
//重点 std::map = 自动排序的“键 → 值”容器（红黑树）
//key  →  value（key 永远有序。）
//map的每一个元素都是一个 pair（键值对）
//map<string,int>m; //key 是 string，value 是 int
int main()
{
    map<string,int>m;
    m["apple"]=5;
    m["banana"]=3;
    auto it=m.find("apple");
    if(it!=m.end())
    cout<<it->first<<" "<<it->second<<endl;
    for(const auto&p:m)//range-for
    {
        cout<<p.first<<" "<<p.second<<endl;
    }
    /*等价于上一个循环
    for (auto it = score.begin(); it != score.end(); ++it)
{
    const auto& p = *it;
    cout << p.first << " " << p.second << endl;
}

    */
    return 0;
}