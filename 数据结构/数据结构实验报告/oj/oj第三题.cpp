#include<stdio.h>
#include<stdlib.h>
typedef struct node       //结点 
{
	float coef;	//系数
	int exp;		//指数
	struct node* next;
}Node, *NodePtr; 		//Node用于声明结点,NodePtr用于声明指针

typedef struct LinkList        //链表 
{
	NodePtr head;
	NodePtr tail;
	int len;
}LinkList,*LinkListPtr;

LinkListPtr CreatNew()     //创建空的新链表 
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
void ListInsert(LinkListPtr q,float coef,int exp)   //在新链表后插入元素 
{
	NodePtr p=(NodePtr)malloc(sizeof(Node));
	p->next=NULL;
	p->exp=exp;
	p->coef=coef;
	q->tail->next=p;
	q->tail=p;
	q->len++;
}

void BubbleSort(LinkListPtr q)   //利用冒泡排序将链表中按exp降序排列 
{
	if(q->len==0) return;
	int count=q->len-1;
	int i=0;              //i为内层循环次数，每次为count 
	float temp_coef=0;
	int temp_exp=0;
	NodePtr a=q->head->next;
	NodePtr b=a->next;
	while(count!=0)      //冒泡排序外层循环 
	{
		i=count;
		while(i!=0)           //冒泡排序内层循环 
		{
			if(a->exp<b->exp)                  //按exp降序 
			{
				temp_exp=a->exp;
				a->exp=b->exp;
				b->exp=temp_exp;
				temp_coef=a->coef;
				a->coef=b->coef;
				b->coef=temp_coef;
			}
			a=b;                       //a，b两个指针后移 
			b=b->next;
			i--;
		}
		count--;
		a=q->head->next;
		b=a->next; 
	}
}

LinkListPtr Merge(LinkListPtr q1,LinkListPtr q2) //将q1和q2合并到一个新链表并返回 
{
	LinkListPtr q3=CreatNew();         //将q1和q2合并到链表q3中
	NodePtr p1=q1->head->next;
	NodePtr p2=q2->head->next;
	while(!((p1==NULL)&&(p2==NULL)))          //只有当p1，p2都指向NULL才退出循环 
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
				//若两个同指数项运算完后，系数正好等于0，则不记录此项 
				p1=p1->next;
				p2=p2->next;
			}
		}
	} 
	return q3;
}
//注意合并函数中只写了＋的情况，在read函数中，若是-号，则coef会存入其相反数 

int Decimal(float coef)//判断coef小数部分是否为0，若为0则返回0，不为0则返回1
{
	int a=coef/1;
	if(coef-a==0) return 0;
	return 1;
} 

void Display(LinkListPtr q3)
{
	if(q3->len==0)                      //先判段特殊情况，空的就输出0 0 
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
		if(coef==0) ;		                     	//输入的“0 0”不存储 
		else if(a=='-') ListInsert(q,-coef,exp);    //减则存入负的coef 
		else ListInsert(q,coef,exp);					
	}
	return x;
}
int main()
{	
	LinkListPtr q1=CreatNew();           //创建两个链表 
	LinkListPtr q2=CreatNew();
	char a=0;                            //最后读入的a来判读是加还是减  
	a=Read(q1,a);						 //将数据读入两个链表 
	Read(q2,a);
	BubbleSort(q1);                      //将两个链表冒泡排序降序排列 
	BubbleSort(q2); 
	LinkListPtr q3=Merge(q1,q2);         //将两个链表合并到第三个链表q3中 
	Display(q3);                         //输出q3 
	return 0;
}

