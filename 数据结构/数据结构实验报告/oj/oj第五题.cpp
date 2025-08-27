#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//���ĸ���������ɴ˹ؿ� 
typedef struct Node          //˫������ڵ� 
{
	char name[20];
	double distance;
	struct Node* pre; 
	struct Node* next;
}Node,*NodePtr;

typedef struct  DoubleLinklist     //˫��ѭ������ 
{
	int line;                //������·�� 
	NodePtr head;
	NodePtr end;
}DoubleLinklist,*DoubleLinklistPtr;

DoubleLinklistPtr CreatNew(int line)        //���������·�� ,����һ���յ�˫������ 
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

void AddNode(DoubleLinklistPtr q,char name[20],double distance)     //˫�������м���ڵ� 
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

void read(DoubleLinklistPtr q)            //�����ݶ���q 
{
	char name[20];
	double distance;
	while(1)
	{
		scanf("%s %lf",name,&distance);
		AddNode(q,name,distance);          //����ڵ� 
		if(distance==0) break;
	}
}
void Display(DoubleLinklistPtr q)              //������� 
{
	NodePtr head=q->head;
	NodePtr temp=head->next;
	printf("%d ",q->line);         
	while(temp!=head)
	{
		if(temp->next==head)                     //�ж��Ƿ񵽱�β���������������0 
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
	int n,line;              //��n����·,��·����Ϊline 
	scanf("%d",&n); 
	DoubleLinklistPtr q[n];       
	int i=0;
	while(i<n)
	{
		scanf("%d",&line);
		q[i]=CreatNew(line);      //���ν�n����·����n��˫�������� 
		read(q[i]); 
		i++;
	}
	i=0;
	while(i<n)
	{
		Display(q[i]);             //�������n����· 
		i++;
	}
	return 0;
}
