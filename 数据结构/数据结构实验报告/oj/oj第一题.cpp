#include<stdio.h>
#include<stdlib.h>
typedef struct QNode                //���ṹ 
{
	int data;
	struct QNode* next;
}QNode,*QueuePtr;
typedef struct LinkQueue            // ���е�����ṹ 
{
	QueuePtr front,rear;
}LinkQueue,*LinkQueuePtr;

LinkQueuePtr CreatNew(int maxsize)         //�ȴ���һ���յ�maxsize��Сѭ������     
{
	QueuePtr head=(QueuePtr)malloc(sizeof(QNode));
	head->data=0;
	head->next=NULL;
	QueuePtr end=head;
	LinkQueuePtr q=(LinkQueuePtr)malloc(sizeof(LinkQueue));
	q->front=head;                      //front��rear��ָ��head 
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
	end->next=head;             //����ѭ�� 
	return q;                
}

void EnQueue(LinkQueuePtr q,int i,int n) //��n��Ԫ�ش��±�i����� 
{
	QueuePtr temp=q->front;
	QueuePtr head=q->front;
	temp=temp->next;         //��tempָ���һ����� 
	while(i!=0)
	{
		temp=temp->next;
		i--;                             //��tempָ���i����� 
	}
	q->front=temp;
	while(n!=0)                          //�ӵ�i�����д�빲n������ 
	{
		if(temp==head)temp=temp->next;    //�������ڵ������� 
		scanf("%d",&temp->data);
		temp=temp->next;               
		n--;
	} 
	q->rear=temp;                         //����rear 
}

int main()
{
	int n,i,k;                   //nΪ����Ԫ�ظ���,iΪ�����±�,kΪ�������
	LinkQueuePtr q=CreatNew(8);  //����һ���յ�8����С��ѭ������ 
	QueuePtr head=q->front;      //��head������ 
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
	EnQueue(q,i,n);                //��n����������� 
	while(k!=0)
	{
		if(q->front==head) q->front=q->front->next;      //����head����������ͷ�ڵ㲻�������� 
		printf("%d ",q->front->data);                    //���k������ 
		q->front=q->front->next;
		k--;
	}
	return 0;
}

