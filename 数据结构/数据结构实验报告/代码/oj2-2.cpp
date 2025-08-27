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
 
DoubleLinklistPtr DeleteStation(DoubleLinklistPtr q,int* flag)
{
	DoubleLinklistPtr qNew=CreatNew(q->line);
	char a[20];
	*flag=0;                //判断是否删除了值 ,flag为0没有删除 
	scanf("%s",a);
	NodePtr temp=q->head->next;
	while(temp!=q->head)
	{
		
		if(!strcmp(temp->name,a))    //如果相等就遍历下一个，不加入到qNew中 
		{
			temp=temp->next;
			*flag=1;                  //标志找到了要删除的站 ，flag为1删除成功 
		}
		if(!strcmp(temp->next->name,a))                //如果下个站点要删去，则这个站点的距离需要修改 
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
	*flag=2;                         //flag=0删除失败；flag=1删除成功或增加成功；flag=2；flag=3增加失败，已有同名站点
	float a,b;
	char c[20],d[20];
	NodePtr temp=q->head->next;
	scanf("%f%f",&a,&b);
	if(a==0)            //即可能在第一个位置加入站点 
	{
		scanf("%s",c) ;//将可能加入的站点读入c
		while(temp!=q->head)
		{
			if(!strcmp(temp->name,c))
			{
				*flag=3;       //增加失败，已有同名站点,将flag设置为3.直接返回 
				return qNew;	
			}
			AddNode(qNew,temp->name,temp->distance);
			temp=temp->next; 	
		}
		*flag=1;                                             
		NodePtr p=(NodePtr)malloc(sizeof(Node));            //没有同名站点，开始插入操作 ,且将flag设置为1 
		strcpy(p->name,c);
		p->distance=b;
		temp=qNew->head->next;
		p->next=temp;
		p->pre=qNew->head;
		temp->pre=p;
		qNew->head->next=p;                  //flag=1,插入成功，正常返回 
		return qNew; 
	}
	else
	{
		scanf("%s%s",c,d);           //在c站点后加入可能添加的d站点
		while(temp!=q->head)
			{
				if(!strcmp(temp->name,d))
				{
					*flag=3;        //增加失败，已有同名站点,将flag设置为3.直接返回 
					return qNew;
				}
				temp=temp->next;
			}	
		temp=q->head->next;
		while(temp!=q->head)            //先将原路线复制到qNew中 
		{
			AddNode(qNew,temp->name,temp->distance);
			temp=temp->next; 
		}
		temp=qNew->head->next;
		while(temp!=qNew->head)   //再在qNew中做插入操作 
		{
			if(!strcmp(temp->name,c))      //找到了c 
			{
				*flag=1;
				temp->distance=a;
				NodePtr p=(NodePtr)malloc(sizeof(Node));            //开始插入操作 
				strcpy(p->name,d);
				p->distance=b;
				NodePtr temp2=temp->next;                    //即在temp和temp2中插入一个p
				p->next=temp2;
				p->pre=temp;
				temp->next=p;
				temp2->pre=p;                 //flag=1,插入成功，正常返回 
				return qNew; 
			}
			temp=temp->next; 
		}                          //flag为2，没有找到c 
		return qNew;	
	} 
		
}

void Display_qNew(DoubleLinklistPtr qNew,int flag)	 //flag=0删除失败；flag=1删除成功或增加成功；                                                        
{                                                    //flag=2，没有与输入的增加位置前一站点同名的站点，flag=3增加失败，已有同名站点
	if(flag==0) printf("删除失败，没有同名站点");
	if(flag==1) Display(qNew);              
	if(flag==2) printf("增加失败，没有与输入的增加位置前一站点同名的站点");
	if(flag==3) printf("增加失败，已有同名站点");
}

int main()
{	
	int n,line,flag;              //有n条线路,线路名字为line,输出的几种情况用flag标识 
	char pattern[20]; 
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
	scanf("%s%d",pattern,&line);
	DoubleLinklistPtr qNew=NULL;             //用一个qNew链表保存被改动后的那条线路的结果 
	for(i=0;i<n;i++)                  //找到对应line的的数组，然后赋给qNew传入函数中 
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
		Display(q[i]);             //依次输出n条线路 
		i++;
	}
	Display_qNew(qNew,flag);        //输出新的修改后的线路 
	return 0;
}
