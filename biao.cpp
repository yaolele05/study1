//顺序表的实现
#include <iostream>
using namespace std;
#define MAX 100
class list
{
    private:
    int data[MAX];
    int length;
    public:
    list():length(0){}
    void insert(int pos, int a)
    {
        if(length>=MAX)
        {
            cout<<"表满"<<endl;
            return;
        }
        if(pos<0||pos>length)
        {
            cout<<"位置不合法"<<endl;
            return ;
        }
        //元素后移
        for(int i=length-1;i>pos;i--)
        {
            data[i]=data[i-1];
        }
        data[pos]=a;
        length++;
    }
    //删除元素，位置后面元素要前移
    void remove(int pos)
    {
        if(length==0||pos>=length||pos<0)
        {
            cout<<"none"<<endl;
            return ;
        }
        for(int i=pos;i<length-1;i++)
         {
           data[i]=data[i+1];      
         }
         length--;
    }
    void print()
    {
        for(int i=0;i<length;i++)
        {
            cout<<data[i]<<" ";
        }
        cout<<endl;
    }
};
/*int main()
{
    list l;
    int n,a,pos;
    cin>>n>>a>>pos;
    l.insert(pos,a);
    l.print();
    l.remove(pos);
    l.print();
    return 0;
}*/
//动态顺序表的实现
#include <iostream>
using namespace std;
class List
{
    private:
    int*arr;
    int size;
    int count;
    public:
    List(int s):size(s),count(0)
    {
        arr=new int[size];
    }
    ~List()
    {
        delete[] arr;
    }
    //添加元素，位置后面元素要后移
    void insert(int pos,int a)
    {
        if(count>=size)
        {
            cout<<"表满"<<endl;
            return;
        }
        if(pos<0||pos>size)
        {
            cout<<"位置不合法"<<endl;
            return ;
        }
        //元素后移
        for(int i=count-1;i>=pos;i--)
        {
            arr[i]=arr[i+1];//或者当i=count,i>pos时，arr[i]=arr[i-1];
        }
        arr[pos]=a;
        count++;
    }
    //删除元素，位置后面元素要前移
    void remove(int pos)
    {
        if(pos<0||pos>size-1||count==0)
        {
            cout<<"none"<<endl;
            return ;
        }
        for(int i=pos;i<count-1;i++)
        {
            arr[i]=arr[i+1];
        }
        count--;
    }
    void print()
    {
        if(count==0)
        {
            cout<<"none"<<endl;
            return ;
        }
        for(int i=0;i<count;i++)
        {
            cout<<arr[i]<<" ";
        }
        cout<<endl;
    }
   //重点：动态顺序表的扩容函数
   void resize()
   {
    size*=2;
    int*newarr=new int[size];
    for(int i=0;i<count;i++)
    {
        newarr[i]=arr[i];
    }
    delete[] arr;
    arr=newarr;

   }
};
int main()
{
    List l(10);
    int m;
    
    for(int i=0;i<10;i++)
    {
        cin>>m;
        l.insert(i,m);
    }
    l.print();
    int n;
    cin>>n;
    l.remove(n);
    l.print();
    return 0;
}