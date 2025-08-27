#include<stdio.h>
#include<stdlib.h>
typedef struct node       //��� 
{
	float coef;	//ϵ��
	int exp;		//ָ��
	struct node* next;
}Node, *NodePtr; 		//Node�����������,NodePtr��������ָ��

typedef struct LinkList        //���� 
{
	NodePtr head;
	NodePtr tail;
	int len;
}LinkList,*LinkListPtr;

LinkListPtr CreatNew()     //�����յ������� 
{
	LinkListPtr q=(LinkListPtr)malloc(sizeof(LinkList));
	NodePtr head=(NodePtr)malloc(sizeof(Node));
	head->next=NULL;
	head->coef=0;
	head->exp=0;
	q->head=head;
	q->tail=head;
	q->len=0;
	return q;
}
void ListInsert(LinkListPtr q,float coef,int exp)   //������������Ԫ�� 
{
	NodePtr p=(NodePtr)malloc(sizeof(Node));
	p->next=NULL;
	p->exp=exp;
	p->coef=coef;
	q->tail->next=p;
	q->tail=p;
	q->len++;
}

void BubbleSort(LinkListPtr q)   //����ð�����������а�exp�������� 
{
	if(q->len==0) return;
	int count=q->len-1;
	int i=0;              //iΪ�ڲ�ѭ��������ÿ��Ϊcount 
	float temp_coef=0;
	int temp_exp=0;
	NodePtr a=q->head->next;
	NodePtr b=a->next;
	while(count!=0)      //ð���������ѭ�� 
	{
		i=count;
		while(i!=0)           //ð�������ڲ�ѭ�� 
		{
			if(a->exp<b->exp)                  //��exp���� 
			{
				temp_exp=a->exp;
				a->exp=b->exp;
				b->exp=temp_exp;
				temp_coef=a->coef;
				a->coef=b->coef;
				b->coef=temp_coef;
			}
			a=b;                       //a��b����ָ����� 
			b=b->next;
			i--;
		}
		count--;
		a=q->head->next;
		b=a->next; 
	}
}

LinkListPtr Merge(LinkListPtr q1,LinkListPtr q2) //��q1��q2�ϲ���һ������������ 
{
	LinkListPtr q3=CreatNew();         //��q1��q2�ϲ�������q3��
	NodePtr p1=q1->head->next;
	NodePtr p2=q2->head->next;
	while(!((p1==NULL)&&(p2==NULL)))          //ֻ�е�p1��p2��ָ��NULL���˳�ѭ�� 
	{
		while(p1==NULL)
		{
			if(p2==NULL)break;
			ListInsert(q3,p2->coef,p2->exp);
			p2=p2->next;
		}
		while(p2==NULL)
		{
			if(p1==NULL)break;
			ListInsert(q3,p1->coef,p1->exp);
			p1=p1->next;
		}
		if(p1!=NULL&&p2!=NULL)
		{
			if(p1->exp>p2->exp)
			{
				ListInsert(q3,p1->coef,p1->exp);
				p1=p1->next;
			}
				if(p1->exp<p2->exp)
			{
				ListInsert(q3,p2->coef,p2->exp);
				p2=p2->next;
			}
			if(p1->exp==p2->exp)
			{	
				if((p1->coef+p2->coef)!=0)ListInsert(q3,p1->coef+p2->coef,p1->exp);
				//������ָͬ�����������ϵ�����õ���0���򲻼�¼���� 
				p1=p1->next;
				p2=p2->next;
			}
		}
	} 
	return q3;
}
//ע��ϲ�������ֻд�ˣ����������read�����У�����-�ţ���coef��������෴�� 

int Decimal(float coef)//�ж�coefС�������Ƿ�Ϊ0����Ϊ0�򷵻�0����Ϊ0�򷵻�1
{
	int a=coef/1;
	if(coef-a==0) return 0;
	return 1;
} 

void Display(LinkListPtr q3)
{
	if(q3->len==0)                      //���ж�����������յľ����0 0 
	{
		printf("0 0");
		return;
	}
	NodePtr p=q3->head->next;	
	while(p!=NULL)
	{
		if(Decimal(p->coef)) printf("%.1f %d ",p->coef,p->exp);
		else printf("%.0f %d ",p->coef,p->exp);
		p=p->next;
	}
}
char Read(LinkListPtr q,char a) 
{
	char x,y;
	double coef;
	int exp;
	while(1)
	{
		x=getchar();
		if(x==' ') x=getchar();			 
		if(x=='-'||x=='+')
		{
			y=getchar();
			if(y!=' ')
			{
				ungetc(y,stdin);
				ungetc(x,stdin);
			}
			else break;
		}
		else if(x=='\n'||x==EOF) break;
		else ungetc(x,stdin);
		scanf("%lf %d",&coef,&exp); 
		if(coef==0) ;		                     	//����ġ�0 0�����洢 
		else if(a=='-') ListInsert(q,-coef,exp);    //������븺��coef 
		else ListInsert(q,coef,exp);					
	}
	return x;
}
int main()
{	
	LinkListPtr q1=CreatNew();           //������������ 
	LinkListPtr q2=CreatNew();
	char a=0;                            //�������a���ж��Ǽӻ��Ǽ�  
	a=Read(q1,a);						 //�����ݶ����������� 
	Read(q2,a);
	BubbleSort(q1);                      //����������ð������������ 
	BubbleSort(q2); 
	LinkListPtr q3=Merge(q1,q2);         //����������ϲ�������������q3�� 
	Display(q3);                         //���q3 
	return 0;
}

