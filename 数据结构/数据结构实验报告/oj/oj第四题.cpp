#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
	NodePtr temp=q->head->next;                      //用一个temp指针遍历整个链表，如果存在和exp相等的项就直接合并
	while(temp!=NULL)                                //这里合并也是默认为加  
	{
		if(temp->exp==exp)
		{
			temp->coef+=coef;
			return;
		}
		temp=temp->next;
	} 
	//没有和exp相等的项，则创建一个新的节点加在链表后面                                                      
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

LinkListPtr Merge(LinkListPtr q1,LinkListPtr q2,char sign) //将q1和q2合并到一个新链表并返回 
{
	LinkListPtr q3=CreatNew();         //将q1和q2合并到链表q3中
	NodePtr p1=q1->head->next;
	NodePtr p2=q2->head->next;
	if(sign=='-')                   //如果是减法，则提前将q2的coef全取相反数 
	{
		while(p2!=NULL)
		{
			p2->coef=0-p2->coef;
			p2=p2->next;
		} 
	}
	p2=q2->head->next;
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
		printf("0");
		return;
	}
	NodePtr p=q3->head->next;	
	while(p!=NULL)
	{
		if(p->exp==0)                   //exp为0时输出是不输出x^的 
		{
			if(p!=q3->head->next)          //p如果是第一项的话，正数则不用输出+ 
			{
				if(p->coef>0)printf("+");   //正号则输出一个+，负号情况下，负数前自带负号，故不用额外输出 
			}
			if(Decimal(p->coef)) printf("%.1f",p->coef);
			else printf("%.0f",p->coef);	
		}
		else
		{
			if(p!=q3->head->next)
			{
				if(p->coef>0)printf("+");   //正号则输出一个+，负号情况下，负数前自带负号，故不用额外输出 
			}
			if(p->coef==1||p->coef==-1)           //如果系数是正负一，则不用输出系数 
			{
				if(p->coef==-1)printf("-");       //-1的话要输出一个负号，1则不用额外输出 
				printf("x");
				if(p->exp!=1)printf("^%d",p->exp);	
			}
			else
			{
				if(Decimal(p->coef)) 
					{
						printf("%.1fx",p->coef);
						if(p->exp!=1)printf("^%d",p->exp);   //指数为1则不输出指数 
					}
					else 
					{
						printf("%.0fx",p->coef);
						if(p->exp!=1)printf("^%d",p->exp);   //指数为1则不输出指数 
					}
			} 
		}
		p=p->next;
	}
}

char* Get_coef_exp(LinkListPtr q, char* sp)    //将sp指向的数组里的字符全部换成coef和exp 
{
	float coef;
	int exp;
	coef=atof(sp);
	if(*sp=='0')return sp+1;
	if(coef==0)
	{
		if(*sp=='-')
		{
			coef=-1;
			sp=sp+2;
		}
		else
		{
			coef=1;
			sp++;
		}
		if(*sp=='^')
		{
			sp++;
			exp=atoi(sp);
			if(*sp=='-')sp++;
			while((*sp>='0')&&(*sp<='9'))sp++;
		}
		else exp=1;
	}
	else 
	{
		if((*sp=='-')||(*sp=='+'))sp++;
		while (((*sp >='0')&&(*sp<='9'))||(*sp=='.'))sp++;
		if(*sp!='x') exp=0;
		else
		{
			sp++;
			if(*sp=='^')
			{
				sp++;
				exp=atoi(sp);
				if(*sp=='-')sp++;
				while((*sp>='0')&&(*sp<='9'))sp++;
			}
			else exp=1;
		}
	}
	ListInsert(q,coef,exp);
	return sp;
}
void Read(LinkListPtr q1, LinkListPtr q2, char* sign)
{
	char str[100];
	char s1[40],s2[40];     
	char* p1=s1;
	char* p2=s2;
	gets(str);                        //将字符串全部读入str中 
	char* pl = str;
	p1=strtok(pl, ")");            //再将两个多项式分别存入p1和p2两个数组 
	p1++;
	p2=strtok(NULL, ")");           
	*sign=p2[0];                    //将运算的是加还是减存入sign，合并的时候会用到 
	p2=p2+2;
	p1=Get_coef_exp(q1,p1);
	while(*p1!=0)
	{
		if(*p1=='+')p1++;
		p1=Get_coef_exp(q1,p1);
	}
	p2=Get_coef_exp(q2,p2);
	while(*p2!=0)
	{
		if(*p2=='+')p2++;
		p2=Get_coef_exp(q2,p2);
	}
	return;
}

int main()
{	
	char sign;
	LinkListPtr q1=CreatNew();           //创建两个链表 
	LinkListPtr q2=CreatNew();
	Read(q1, q2, &sign);                 //将字符串读入两个链表中 
	BubbleSort(q1);                      //将两个链表冒泡排序降序排列 
	BubbleSort(q2); 
	LinkListPtr q3=Merge(q1,q2,sign);     //将两个链表合并到第三个链表q3中 
	Display(q3);                         //输出q3 
	return 0;
}

