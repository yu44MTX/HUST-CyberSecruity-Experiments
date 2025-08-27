#include<stdio.h>
#include<stdlib.h>
typedef struct QNode                //结点结构 
{
	int data;
	struct QNode* next;
}QNode,*QueuePtr;
typedef struct LinkQueue            // 队列的链表结构 
{
	QueuePtr front,rear;
}LinkQueue,*LinkQueuePtr;

LinkQueuePtr CreatNew(int maxsize)         //先创立一个空的maxsize大小循环链表     
{
	QueuePtr head=(QueuePtr)malloc(sizeof(QNode));
	head->data=0;
	head->next=NULL;
	QueuePtr end=head;
	LinkQueuePtr q=(LinkQueuePtr)malloc(sizeof(LinkQueue));
	q->front=head;                      //front和rear都指向head 
	q->rear=head;                  
	while(maxsize!=0)                 
	{
		QueuePtr p=(QueuePtr)malloc(sizeof(QNode));        
		p->next=NULL;
		p->data=0; 
		end->next=p;
		end=p;
		maxsize--;
	}
	end->next=head;             //让其循环 
	return q;                
}

void EnQueue(LinkQueuePtr q,int i,int n) //将n个元素从下标i入队列 
{
	QueuePtr temp=q->front;
	QueuePtr head=q->front;
	temp=temp->next;         //将temp指向第一个结点 
	while(i!=0)
	{
		temp=temp->next;
		i--;                             //将temp指向第i个结点 
	}
	q->front=temp;
	while(n!=0)                          //从第i个结点写入共n个数据 
	{
		if(temp==head)temp=temp->next;    //遇到都节点则跳过 
		scanf("%d",&temp->data);
		temp=temp->next;               
		n--;
	} 
	q->rear=temp;                         //设置rear 
}

int main()
{
	int n,i,k;                   //n为读入元素个数,i为队首下标,k为输出个数
	LinkQueuePtr q=CreatNew(8);  //创立一个空的8个大小的循环链表 
	QueuePtr head=q->front;      //将head存下来 
	scanf("%d%d%d",&n,&i,&k) ;
	if(k>8)
	{
		printf("Error");
		return 0;
	}
	if(k>n)
	{
		printf("Fault");
		return 0;
	}
	if(n>8)
	{
		printf("Full ");
		n=8;
	}
	EnQueue(q,i,n);                //将n个数据入队列 
	while(k!=0)
	{
		if(q->front==head) q->front=q->front->next;      //遇到head，则跳过，头节点不存入数据 
		printf("%d ",q->front->data);                    //输出k个数据 
		q->front=q->front->next;
		k--;
	}
	return 0;
}

