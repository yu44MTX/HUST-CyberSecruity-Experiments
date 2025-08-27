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
 
DoubleLinklistPtr DeleteStation(DoubleLinklistPtr q,int* flag)
{
	DoubleLinklistPtr qNew=CreatNew(q->line);
	char a[20];
	*flag=0;                //�ж��Ƿ�ɾ����ֵ ,flagΪ0û��ɾ�� 
	scanf("%s",a);
	NodePtr temp=q->head->next;
	while(temp!=q->head)
	{
		
		if(!strcmp(temp->name,a))    //�����Ⱦͱ�����һ���������뵽qNew�� 
		{
			temp=temp->next;
			*flag=1;                  //��־�ҵ���Ҫɾ����վ ��flagΪ1ɾ���ɹ� 
		}
		if(!strcmp(temp->next->name,a))                //����¸�վ��Ҫɾȥ�������վ��ľ�����Ҫ�޸� 
		{
			AddNode(qNew,temp->name,temp->distance+temp->next->distance);
		} 
		else AddNode(qNew,temp->name,temp->distance);
		temp=temp->next; 
	}          
	return qNew;
}

DoubleLinklistPtr AddStation(DoubleLinklistPtr q,int* flag)
{
	DoubleLinklistPtr qNew=CreatNew(q->line);
	*flag=2;                         //flag=0ɾ��ʧ�ܣ�flag=1ɾ���ɹ������ӳɹ���flag=2��flag=3����ʧ�ܣ�����ͬ��վ��
	float a,b;
	char c[20],d[20];
	NodePtr temp=q->head->next;
	scanf("%f%f",&a,&b);
	if(a==0)            //�������ڵ�һ��λ�ü���վ�� 
	{
		scanf("%s",c) ;//�����ܼ����վ�����c
		while(temp!=q->head)
		{
			if(!strcmp(temp->name,c))
			{
				*flag=3;       //����ʧ�ܣ�����ͬ��վ��,��flag����Ϊ3.ֱ�ӷ��� 
				return qNew;	
			}
			AddNode(qNew,temp->name,temp->distance);
			temp=temp->next; 	
		}
		*flag=1;                                             
		NodePtr p=(NodePtr)malloc(sizeof(Node));            //û��ͬ��վ�㣬��ʼ������� ,�ҽ�flag����Ϊ1 
		strcpy(p->name,c);
		p->distance=b;
		temp=qNew->head->next;
		p->next=temp;
		p->pre=qNew->head;
		temp->pre=p;
		qNew->head->next=p;                  //flag=1,����ɹ����������� 
		return qNew; 
	}
	else
	{
		scanf("%s%s",c,d);           //��cվ�����������ӵ�dվ��
		while(temp!=q->head)
			{
				if(!strcmp(temp->name,d))
				{
					*flag=3;        //����ʧ�ܣ�����ͬ��վ��,��flag����Ϊ3.ֱ�ӷ��� 
					return qNew;
				}
				temp=temp->next;
			}	
		temp=q->head->next;
		while(temp!=q->head)            //�Ƚ�ԭ·�߸��Ƶ�qNew�� 
		{
			AddNode(qNew,temp->name,temp->distance);
			temp=temp->next; 
		}
		temp=qNew->head->next;
		while(temp!=qNew->head)   //����qNew����������� 
		{
			if(!strcmp(temp->name,c))      //�ҵ���c 
			{
				*flag=1;
				temp->distance=a;
				NodePtr p=(NodePtr)malloc(sizeof(Node));            //��ʼ������� 
				strcpy(p->name,d);
				p->distance=b;
				NodePtr temp2=temp->next;                    //����temp��temp2�в���һ��p
				p->next=temp2;
				p->pre=temp;
				temp->next=p;
				temp2->pre=p;                 //flag=1,����ɹ����������� 
				return qNew; 
			}
			temp=temp->next; 
		}                          //flagΪ2��û���ҵ�c 
		return qNew;	
	} 
		
}

void Display_qNew(DoubleLinklistPtr qNew,int flag)	 //flag=0ɾ��ʧ�ܣ�flag=1ɾ���ɹ������ӳɹ���                                                        
{                                                    //flag=2��û�������������λ��ǰһվ��ͬ����վ�㣬flag=3����ʧ�ܣ�����ͬ��վ��
	if(flag==0) printf("ɾ��ʧ�ܣ�û��ͬ��վ��");
	if(flag==1) Display(qNew);              
	if(flag==2) printf("����ʧ�ܣ�û�������������λ��ǰһվ��ͬ����վ��");
	if(flag==3) printf("����ʧ�ܣ�����ͬ��վ��");
}

int main()
{	
	int n,line,flag;              //��n����·,��·����Ϊline,����ļ��������flag��ʶ 
	char pattern[20]; 
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
	scanf("%s%d",pattern,&line);
	DoubleLinklistPtr qNew=NULL;             //��һ��qNew�����汻�Ķ����������·�Ľ�� 
	for(i=0;i<n;i++)                  //�ҵ���Ӧline�ĵ����飬Ȼ�󸳸�qNew���뺯���� 
	{
		if(q[i]->line==line)         
		{
			qNew=q[i];
			break;
		}
	}
	if(!strcmp(pattern,"add")) qNew=AddStation(qNew,&flag);
	if(!strcmp(pattern,"delete")) qNew=DeleteStation(qNew,&flag);               
	i=0;
	while(i<n)
	{
		Display(q[i]);             //�������n����· 
		i++;
	}
	Display_qNew(qNew,flag);        //����µ��޸ĺ����· 
	return 0;
}
