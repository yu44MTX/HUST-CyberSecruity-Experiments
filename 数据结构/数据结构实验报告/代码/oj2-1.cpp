#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//用四个链表来完成此关卡 
typedef struct Node          //双向链表节点 
{
	char name[20];
	double distance;
	struct Node* pre; 
	struct Node* next;
}Node,*NodePtr;

typedef struct  DoubleLinklist     //双向循环链表 
{
	int line;                //地铁线路号 
	NodePtr head;
	NodePtr end;
}DoubleLinklist,*DoubleLinklistPtr;

DoubleLinklistPtr CreatNew(int line)        //输入地铁线路号 ,创建一个空的双向链表 
{
	DoubleLinklistPtr q=(DoubleLinklistPtr)malloc(sizeof(DoubleLinklist));
	q->line=line;
	NodePtr head=(NodePtr)malloc(sizeof(Node));
	head->distance=0;
	head->next=NULL;
	head->pre==NULL;
	q->head=head; 
	q->end=head;
	return q;
}

void AddNode(DoubleLinklistPtr q,char name[20],double distance)     //双向链表中加入节点 
{
	NodePtr p=(NodePtr)malloc(sizeof(Node));
	strcpy(p->name,name);
	p->distance=distance;
	q->end->next=p;
	p->pre=q->end;
	p->next=q->head;
	q->head->pre=p;
	q->end=p;
}

void read(DoubleLinklistPtr q)            //将数据读入q 
{
	char name[20];
	double distance;
	while(1)
	{
		scanf("%s %lf",name,&distance);
		AddNode(q,name,distance);          //读入节点 
		if(distance==0) break;
	}
}
void Display(DoubleLinklistPtr q)              //输出数据 
{
	NodePtr head=q->head;
	NodePtr temp=head->next;
	printf("%d ",q->line);         
	while(temp!=head)
	{
		if(temp->next==head)                     //判断是否到表尾，若是则不输出距离0 
		{
			printf("%s\n",temp->name);
			break;
		}
		printf("%s %.2f ",temp->name,temp->distance);
		temp=temp->next;
	}
}
int main()
{	
	int n,line;              //有n条线路,线路名字为line 
	scanf("%d",&n); 
	DoubleLinklistPtr q[n];       
	int i=0;
	while(i<n)
	{
		scanf("%d",&line);
		q[i]=CreatNew(line);      //依次将n条线路读入n条双向链表中 
		read(q[i]); 
		i++;
	}
	i=0;
	while(i<n)
	{
		Display(q[i]);             //依次输出n条线路 
		i++;
	}
	return 0;
}
