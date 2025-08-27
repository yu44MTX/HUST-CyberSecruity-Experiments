//Your code begin.
//ʾ�������ο�����Ҳ���������޸���ơ�
#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
	int coef;	//ϵ��
	int exp;		//ָ��
	struct node* next;
}Snode, *StackPtr; 		//Snode�����������,StackPtr��������ջָ��

typedef struct                    //ջ������ṹ 
{
	StackPtr top;
	int count;
}LinkStack,*LinkStackPtr;

LinkStackPtr CreatNew()                //����һ����ջ 
{
	LinkStackPtr q=(LinkStackPtr)malloc(sizeof(LinkStack));
	q->count=0;
	q->top=NULL;
	return q;
}

void push(LinkStackPtr q,int coef,int exp)        //��������ջ 
{
	StackPtr p=(StackPtr)malloc(sizeof(Snode));
	p->coef=coef;
	p->exp=exp;
	p->next=q->top;
	q->top=p;
	q->count++;	
}
int pop(LinkStackPtr q)                 //�����ݳ�ջ����� 
{
	if(q->count==0) return 0;
	printf("%d %d ",q->top->coef,q->top->exp);
	q->top=q->top->next;
	q->count--;
	return 1;
}
void read(LinkStackPtr q)
{                 //��ȡ��������ݣ�ÿ�ֶ�ȡһ��ϵ����һ��ָ��
    int temp=0;
    char c=0;
    int coef,exp;
    while(1)
	{
        if(c=='\n') break;              //����ѭ�����ڣ����ֻ���������ѭ��
        c=getchar();
        if(c==';') break;                  //����ѭ�����ڣ����ַֺ�������ѭ��
        while(c!=' ')                       //���ַ�ת��Ϊ����
		{               
            temp*=10;
            temp+=c-'0';
            c=getchar();
        }
        c=getchar();
        coef=temp;
        temp=0;
        while(c!=' '&&c!='\n')        //���ַ�ת��Ϊ����
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
    getchar();          //ȥ���ֺ�����һ������֮��Ŀո�
    read(q2);
	if(q1->count>5||q2->count>5)        //���ж��Ƿ񳬳���ջ������ 
	{
		printf("Full!");
		return 0; 
	}
	while(!(q1->count==0&&q2->count==0)) 
	{	
		if(q1->top->exp>q2->top->exp) pop(q1);    //��������ջ��exp��С�������
		else pop(q2);
		if(q1->count==0)                      //��q1��ջ�ף���һֱ���q2����q2����Ϊֹ
		{
			while(1)
			{
				pop(q2);
				if(q2->count==0) break;
			}
		}
		if(q2->count==0)                      //��q2��ջ�ף���һֱ���q1����q1����Ϊֹ
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
