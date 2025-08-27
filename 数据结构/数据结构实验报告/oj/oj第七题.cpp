#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXVEX 21
#define INFINITY 65535
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

typedef char VertexType[20];             //顶点类型为字符串，因为要传入站名 
typedef float EdgeType;
typedef struct                         //用邻接矩阵建立图 
{
	VertexType vexs[MAXVEX];
	EdgeType arc[MAXVEX][MAXVEX];
	int numVertexes,numEdges;	 
}MGraph;

typedef int Patharc[MAXVEX];                    //用于存储最短路径下标的数组
typedef float ShortPathTable[MAXVEX];          //用于存储到各点最短路径的权值和 

DoubleLinklistPtr CreatDoubleLinklist(int line)        //输入地铁线路号 ,创建一个空的双向链表 
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

void transfer(int* i,int* j,char a[],char b[],VertexType vexs[])  //将a，b在顶点表中的下标找到，并以i，j返回 
{
	int k=0;
	for(k=0;k<MAXVEX;k++)
	{
		if(!strcmp(a,vexs[k])) *i=k;
		if(!strcmp(b,vexs[k])) *j=k;
	} 
}

void CreatMGraph(MGraph *G,DoubleLinklistPtr q[])         //将四个链表创立一个图 
{
	G->numEdges=0;
	G->numVertexes=0;
	int i,j,k;
	int flag=1;                 //flag=0则表示之前有这个站了，不用加入顶点表 
	//建立顶点表 
	for(i=0;i<MAXVEX;)        
	{
		for(j=0;j<4;j++)         //依次将四个链表读入 
		{
			NodePtr head=q[j]->head;
			NodePtr temp=q[j]->head->next;
			while(temp!=head)
			{
				for(k=0;k<G->numVertexes;k++)
				{
					if(!strcmp(G->vexs[k],temp->name))
					{
						flag=0; 
						break;
					} 
				}
				if(flag==1)                  //之前没有相等的站 
				{
					strcpy(G->vexs[i],temp->name);
					G->numVertexes++;
					i++;
				}
				temp=temp->next;
				flag=1;
			}
		} 
	}
	//初始化邻接矩阵
	for(i=0;i<G->numVertexes;i++)
	{
		for(j=0;j<G->numVertexes;j++)
		{
			G->arc[i][j]=INFINITY;	
		}	
	}
	//建立邻接矩阵
	for(k=0;k<4;k++)        //将四个链表的边是关系输入邻接矩阵中 
	{
		NodePtr head=q[k]->head;
		NodePtr temp=q[k]->head->next;
		while(temp->next!=head) 
		{
			transfer(&i,&j,temp->name,temp->next->name,G->vexs);
			G->arc[i][j]=temp->distance;
			G->arc[j][i]=temp->distance;
			G->numEdges++;
			temp=temp->next;
		}
	}	
}

void ShortestPath_Dijkstra(MGraph G, int v0, Patharc *P, ShortPathTable *D)
{    
	int v,w,k,min;    
	int final[MAXVEX];                        // final[w]=1表示求得顶点v0至vw的最短路径 
	for(v=0; v<G.numVertexes; v++)             //初始化数据 
	{        
		final[v] = 0;		             	//全部顶点初始化为未知最短路径状态 
		(*D)[v] = G.arc[v0][v];             //将与v0点有连线的顶点加上权值 
		(*P)[v] = -1;			          	//初始化路径数组P为-1   
	}

	(*D)[v0] = 0;                         //v0至v0路径为0 
	final[v0] = 1;                         // v0至v0不需要求路径        
    //开始主循环，每次求得v0到某个v顶点的最短路径  
	for(v=1; v<G.numVertexes; v++)   
	{
		min=INFINITY;                          //当前所知离v0顶点的最近距离       
		for(w=0; w<G.numVertexes; w++)           //寻找离v0最近的顶点    
		{            
			if(!final[w] && (*D)[w]<min)             
			{                   
				k=w;                    
				min = (*D)[w];    //w顶点离v0顶点更近             
			}        
		}        
		final[k] = 1;                                //将目前找到的最近的顶点置为1 
		for(w=0; w<G.numVertexes; w++)               // 修正当前最短路径及距离 
		{
			//如果经过v顶点的路径比现在这条路径的长度短的话 
			if(!final[w] && (min+G.arc[k][w]<(*D)[w]))   
			{                                            // 说明找到了更短的路径，修改D[w]和P[w] 
				(*D)[w] = min + G.arc[k][w];             //修改当前路径长度                
				(*P)[w]=k;        
			}       
		}   
	}
}
//Dijkstra算法最后得到的结果为：
//顶点a（下标）到其余顶点v的最短路径P[v]以及带权长度D[v]
// P[v]的值为前驱顶点下标，D[v]表示a到v的最短路径之和 

void InvertedSequence(int a[],int n)           //将有n个元素的数组逆序排列 
{
	int temp=0;
	int i=0;
	for(;i<n-1;i++,n--)
	{
		temp=a[i];
		a[i]=a[n-1];
		a[n-1]=temp;	
	}	
} 

int main()
{	
	int n,line;              //有n条线路,线路名字为line 
	scanf("%d",&n); 
	DoubleLinklistPtr q[n];       
	int i=0;                //i就起一个计数的作用 
	while(i<n)
	{
		scanf("%d",&line);
		q[i]=CreatDoubleLinklist(line);      //依次将n条线路读入n条双向链表中 
		read(q[i]); 
		i++;
	}
	char origin[20],terminal[20];           //起点和终点 
	scanf("%s%s",origin,terminal);
	MGraph G;
	CreatMGraph(&G,q);
	int a,b;                  //用a，b接受起始和终点对应的数组下标 
	Patharc P;                //用于存储最短路径下标的数组
	ShortPathTable D;         //用于存储到各点最短路径的权值和
	transfer(&a,&b,origin,terminal,G.vexs);   
	ShortestPath_Dijkstra(G,a,&P,&D);
	printf("%.2f ",D[b]);         //将到b的总路程输出 
	int m=0;
	int b_temp1=b;                //b的值需要保存下来，所以后面两个要用到b的地方，分别用b_temp1和b_temp2 
	int b_temp2=b; 
	while(P[b_temp1]!=-1)                //判断中间经过了几个站点 ,记录在m中 
		{
			m++;
			b_temp1=P[b_temp1];
		}
	int index[m];
	i=0; 
	while(P[b_temp2]!=-1)              //将i个站点的index逆序存入数组中   
		{
			index[i]=P[b_temp2];
			b_temp2=P[b_temp2];
			i++;
		}
	InvertedSequence(index,m);      //将数组逆序，变成正常的顺序 
	printf("%s %.2f ",origin,G.arc[a][index[0]]);
	for(i=0;i<m-1;i++)
	{
		printf("%s %.2f ",G.vexs[index[i]],G.arc[index[i]][index[i+1]]);
	}
	printf("%s %.2f ",G.vexs[index[m-1]],G.arc[index[m-1]][b]);
	printf("%s",terminal);
	return 0;
}
