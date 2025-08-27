//Your code begin.
//示例仅供参考，你也可以自行修改设计。
#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
	int coef;	//系数
	int exp;		//指数
	struct node* next;
}Snode, *StackPtr; 		//Snode用于声明结点,StackPtr用于声明栈指针

typedef struct                    //栈的链表结构 
{
	StackPtr top;
	int count;
}LinkStack,*LinkStackPtr;

LinkStackPtr CreatNew()                //创建一个空栈 
{
	LinkStackPtr q=(LinkStackPtr)malloc(sizeof(LinkStack));
	q->count=0;
	q->top=NULL;
	return q;
}

void push(LinkStackPtr q,int coef,int exp)        //让数据入栈 
{
	StackPtr p=(StackPtr)malloc(sizeof(Snode));
	p->coef=coef;
	p->exp=exp;
	p->next=q->top;
	q->top=p;
	q->count++;	
}
int pop(LinkStackPtr q)                 //将数据出栈并输出 
{
	if(q->count==0) return 0;
	printf("%d %d ",q->top->coef,q->top->exp);
	q->top=q->top->next;
	q->count--;
	return 1;
}
void read(LinkStackPtr q)
{                 //读取输入的数据，每轮读取一个系数和一个指数
    int temp=0;
    char c=0;
    int coef,exp;
    while(1)
	{
        if(c=='\n') break;              //设置循环出口，出现换行则跳出循环
        c=getchar();
        if(c==';') break;                  //设置循环出口，出现分号则跳出循环
        while(c!=' ')                       //将字符转化为整数
		{               
            temp*=10;
            temp+=c-'0';
            c=getchar();
        }
        c=getchar();
        coef=temp;
        temp=0;
        while(c!=' '&&c!='\n')        //将字符转化为整数
		{   
            temp*=10;
            temp+=c-'0';
            c=getchar();        
        }
        exp=temp;
        temp=0;
        push(q,coef,exp);
    }
}
int main(void)
{
	LinkStackPtr q1=CreatNew();
	LinkStackPtr q2=CreatNew();
	int coef,exp;
	read(q1);
    getchar();          //去掉分号与下一组数据之间的空格
    read(q2);
	if(q1->count>5||q2->count>5)        //先判断是否超出了栈的容量 
	{
		printf("Full!");
		return 0; 
	}
	while(!(q1->count==0&&q2->count==0)) 
	{	
		if(q1->top->exp>q2->top->exp) pop(q1);    //根据两个栈顶exp大小依次输出
		else pop(q2);
		if(q1->count==0)                      //若q1到栈底，则一直输出q2，到q2触底为止
		{
			while(1)
			{
				pop(q2);
				if(q2->count==0) break;
			}
		}
		if(q2->count==0)                      //若q2到栈底，则一直输出q1，到q1触底为止
		{
			while(1)
			{
				pop(q1);
				if(q1->count==0) break;
			}
		}
	}
    return 0;
}
//Your code end.
