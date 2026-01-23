#include <iostream>
using namespace std;
class queue
{
    private:
    int arr[100];
    int front;
    int end;
    public:
    queue():front(0),end(0){}
    void enqueue(int a)
    {
        int b;
        for(int i=0;i<a;i++)
        {
            b=0;
           cin>>b;
            arr[end++]=b;
        }

    }
    void dequeue()
    {
        for(int i=end;i>0;i--)
        {
            cout<<arr[end--]<<" ";
        }
    }
    int seek(int a)
    {
        if(front==end)
        {
            cout<<"none"<<endl;
            return -1;
        }
        else 
        {
            while(front<a)
            {
                front++;
                if(arr[front]==a)
                {
                    return front+1;
                }
            }
            
        }
        }
    };
    int main()
    {
        queue q;
        int n;
        cin>>n;
        q.enqueue(n);
        q.dequeue();
        int x;
        cin>>x;
        cout<<q.seek(x)<<endl;
        return 0;

    }
