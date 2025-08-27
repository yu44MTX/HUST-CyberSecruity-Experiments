#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
	NodePtr temp=q->head->next;                      //��һ��tempָ�������������������ں�exp��ȵ����ֱ�Ӻϲ�
	while(temp!=NULL)                                //����ϲ�Ҳ��Ĭ��Ϊ��  
	{
		if(temp->exp==exp)
		{
			temp->coef+=coef;
			return;
		}
		temp=temp->next;
	} 
	//û�к�exp��ȵ���򴴽�һ���µĽڵ�����������                                                      
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

LinkListPtr Merge(LinkListPtr q1,LinkListPtr q2,char sign) //��q1��q2�ϲ���һ������������ 
{
	LinkListPtr q3=CreatNew();         //��q1��q2�ϲ�������q3��
	NodePtr p1=q1->head->next;
	NodePtr p2=q2->head->next;
	if(sign=='-')                   //����Ǽ���������ǰ��q2��coefȫȡ�෴�� 
	{
		while(p2!=NULL)
		{
			p2->coef=0-p2->coef;
			p2=p2->next;
		} 
	}
	p2=q2->head->next;
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
		printf("0");
		return;
	}
	NodePtr p=q3->head->next;	
	while(p!=NULL)
	{
		if(p->exp==0)                   //expΪ0ʱ����ǲ����x^�� 
		{
			if(p!=q3->head->next)          //p����ǵ�һ��Ļ��������������+ 
			{
				if(p->coef>0)printf("+");   //���������һ��+����������£�����ǰ�Դ����ţ��ʲ��ö������ 
			}
			if(Decimal(p->coef)) printf("%.1f",p->coef);
			else printf("%.0f",p->coef);	
		}
		else
		{
			if(p!=q3->head->next)
			{
				if(p->coef>0)printf("+");   //���������һ��+����������£�����ǰ�Դ����ţ��ʲ��ö������ 
			}
			if(p->coef==1||p->coef==-1)           //���ϵ��������һ���������ϵ�� 
			{
				if(p->coef==-1)printf("-");       //-1�Ļ�Ҫ���һ�����ţ�1���ö������ 
				printf("x");
				if(p->exp!=1)printf("^%d",p->exp);	
			}
			else
			{
				if(Decimal(p->coef)) 
					{
						printf("%.1fx",p->coef);
						if(p->exp!=1)printf("^%d",p->exp);   //ָ��Ϊ1�����ָ�� 
					}
					else 
					{
						printf("%.0fx",p->coef);
						if(p->exp!=1)printf("^%d",p->exp);   //ָ��Ϊ1�����ָ�� 
					}
			} 
		}
		p=p->next;
	}
}

char* Get_coef_exp(LinkListPtr q, char* sp)    //��spָ�����������ַ�ȫ������coef��exp 
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
	gets(str);                        //���ַ���ȫ������str�� 
	char* pl = str;
	p1=strtok(pl, ")");            //�ٽ���������ʽ�ֱ����p1��p2�������� 
	p1++;
	p2=strtok(NULL, ")");           
	*sign=p2[0];                    //��������Ǽӻ��Ǽ�����sign���ϲ���ʱ����õ� 
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
	LinkListPtr q1=CreatNew();           //������������ 
	LinkListPtr q2=CreatNew();
	Read(q1, q2, &sign);                 //���ַ����������������� 
	BubbleSort(q1);                      //����������ð������������ 
	BubbleSort(q2); 
	LinkListPtr q3=Merge(q1,q2,sign);     //����������ϲ�������������q3�� 
	Display(q3);                         //���q3 
	return 0;
}

